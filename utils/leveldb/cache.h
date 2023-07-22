#pragma onece
#include <string>
#include <mutex>
#include <cassert>
#include <cstring>
#include "hash.h"

#ifndef GUARDED_BY
#define GUARDED_BY(x) __attribute__((x))
#endif

#ifndef EXCLUSIVE_LOCKS_REQUIRED
#define EXCLUSIVE_LOCKS_REQUIRED(...) \
   __attribute__((exclusive_locks_required(__VA_ARGS__)))
#endif

class Cache {
public:
    Cache() = default;
    Cache(const Cache&) = delete;
    Cache& operator=(const Cache&) = delete;
    virtual ~Cache() = default;

    struct Handle {};
    
    virtual Handle* Insert(const std::string& key, void* value,
                            size_t charge, void(*deleter)(const std::string& key, void* value)) = 0;
    virtual Handle* Lookup(const std::string& key) = 0;
    virtual void Release(Handle* handle) = 0;
    virtual void* Value(Handle* handle) = 0;
    virtual void Erase(const std::string& key) = 0;
    virtual uint64_t NewId() = 0;
    virtual void Prune() {}
    virtual size_t TotalCharge() const = 0;
};

struct LRUNode {
    void* value;
    void (*deleter)(const std::string&, void* value);
    LRUNode* next_hash;
    LRUNode* next;
    LRUNode* prev;
    size_t charge;
    size_t key_length;
    bool in_cache;
    uint32_t refs;
    uint32_t hash;
    char key_data[1];

    std::string key() const {
        assert(next != this);
        return std::string(key_data, key_length);
    }
};

class LRUTable {
public:
    LRUTable(): length_(0), elems_(0), list_(nullptr) { Resize(); }
    ~LRUTable() { delete[] list_; }

    LRUNode* Lookup(const std::string& key, uint32_t hash) {
        return *FindPointer(key, hash);
    }

    LRUNode* Insert(LRUNode* h) {
        LRUNode **ptr = FindPointer(h->key(), h->hash);
        LRUNode *old = *ptr;
        h->next_hash = (old == nullptr ? nullptr : old->next_hash);
        *ptr = h;
        if (old == nullptr) {
            ++elems_;
            if (elems_ > length_) {
                Resize();
            }
        }
        return old;
    }

    LRUNode* Remove(const std::string& key, uint32_t hash) {
        LRUNode** ptr = FindPointer(key, hash);
        LRUNode *result = *ptr;
        if (result != nullptr) {
            *ptr = result->next_hash;
            --elems_;
        }
        return result;
    }

private:
    LRUNode** FindPointer(const std::string& key, uint32_t hash) {
        LRUNode **ptr = &list_[hash & (length_ - 1)];
        while (*ptr != nullptr && ((*ptr)->hash != hash || key != (*ptr)->key()))
        {
            ptr = &(*ptr)->next_hash;
        }
        return ptr;
    }

    void Resize() {
        uint32_t new_length = 4;
        while (new_length < elems_) {
            new_length *= 2;
        }

        LRUNode** new_list = new LRUNode*[new_length];
        memset(new_list, 0, sizeof(new_list[0]) * new_length);
        uint32_t count = 0;
        for (uint32_t i = 0; i < length_; i++) {
            LRUNode* h = list_[i];
            while (h != nullptr) {
                LRUNode *next = h->next_hash;
                uint32_t hash = h->hash;
                LRUNode **ptr = &new_list[hash & (length_ - 1)];
                h->next_hash = *ptr;
                *ptr = h;
                h = next;
                count++;
            }
        }
        assert(elems_ == count);
        delete[] list_;
        list_ = new_list;
        length_ = new_length;
    }

private:
    uint32_t length_;
    uint32_t elems_;
    LRUNode** list_;
};

class LRUCache {
public:
    LRUCache();
    ~LRUCache();

    void SetCapacity(size_t capacity) { capacity_ = capacity; }
    Cache::Handle* Insert(const std::string& key, uint32_t hash, void* value,
                        size_t charge,
                        void (*deleter)(const std::string& key, void* value));
    Cache::Handle* Lookup(const std::string& key, uint32_t hash);
    void Release(Cache::Handle* node);
    void Erase(const std::string& key, uint32_t hash);
    void Prune();
    size_t TotalCharge() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return usage_;
    }

private:
    void LRU_Remove(LRUNode* e);
    void LRU_Append(LRUNode* list, LRUNode* e);
    void Ref(LRUNode* e);
    void Unref(LRUNode* e);
    bool FinishErase(LRUNode* e) EXCLUSIVE_LOCKS_REQUIRED(mutex_);

private:
    size_t capacity_;
    mutable std::mutex mutex_;
    size_t usage_ GUARDED_BY(mutex_);
    LRUNode lru_ GUARDED_BY(mutex_);
    LRUNode in_usage_ GUARDED_BY(mutex_);
    LRUTable table_ GUARDED_BY(mutex_);
};

static const int kNumSharedBits = 4;
static const int kNumShares = 1 << kNumSharedBits;
class SharedLRUCache: public Cache {
public:
    static inline uint32_t LRUHash(const std::string s) {
        return Hash(s.data(), s.size(), 0);
    }

    static uint32_t Shared(uint32_t hash) { hash >> (32 - kNumSharedBits); }

    explicit SharedLRUCache(size_t capacity): last_id_(0) {
        const size_t per_shared = (capacity + (kNumShares - 1)) / kNumShares;
        for (int s = 0; s < kNumShares; s++) {
            shared_[s].SetCapacity(per_shared);
        }
    }
    ~SharedLRUCache() override {}
    Handle* Insert(const std::string& key, void* value, size_t charge,
                 void (*deleter)(const std::string& key, void* value)) {
        const uint32_t hash = LRUHash(key);
        return shared_[Shared(hash)].Insert(key, hash, value, charge, deleter);
    }

    Handle* Lookup(const std::string& key) override {
        const uint32_t hash = LRUHash(key);
        return shared_[Shared(hash)].Lookup(key, hash);
    }
  void Release(Handle* handle) override {
    LRUNode* h = reinterpret_cast<LRUNode*>(handle);
    shared_[Shared(h->hash)].Release(handle);
  }
  void Erase(const std::string& key) override {
    const uint32_t hash = LRUHash(key);
    shared_[Shared(hash)].Erase(key, hash);
  }
  void* Value(Handle* handle) override {
    return reinterpret_cast<LRUNode*>(handle)->value;
  }
  uint64_t NewId() override {
    std::lock_guard<std::mutex> lock(id_mutex_);
    return ++(last_id_);
  }
  void Prune() override {
    for (int s = 0; s < kNumShares; s++) {
      shared_[s].Prune();
    }
  }
  size_t TotalCharge() const override {
    size_t total = 0;
    for (int s = 0; s < kNumShares; s++) {
      total += shared_[s].TotalCharge();
    }
    return total;
  }
    
private:
    LRUCache shared_[kNumShares];
    std::mutex id_mutex_;
    uint64_t last_id_;
};