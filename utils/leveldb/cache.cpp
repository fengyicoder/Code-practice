#include "cache.h"

LRUCache::LRUCache(): capacity_(0), usage_(0) {
    lru_.next = &lru_;
    lru_.prev = &lru_;
    in_usage_.next = &in_usage_;
    in_usage_.prev = &in_usage_;
}

LRUCache::~LRUCache() {
    assert(in_usage_.next == &in_usage_);
    for (LRUNode *e = lru_.next; e != &lru_; ) {
        LRUNode *next = e->next;
        assert(e->in_cache);
        e->in_cache = false;
        assert(e->refs == 1);
        Unref(e);
        e = next;
    }
}

void LRUCache::Ref(LRUNode* e) {
    if (e->refs == 1 && e->in_cache) {
        LRU_Remove(e);
        LRU_Append(&in_usage_, e);
    }
    e->refs++;
}

void LRUCache::Unref(LRUNode* e) {
    assert(e->refs > 0);
    e->refs--;
    if (e->refs == 0) {
        assert(!e->in_cache);
        (*e->deleter)(e->key(), e->value);
        free(e);
    } else if (e->in_cache && e->refs == 1) {
        LRU_Remove(e);
        LRU_Append(&lru_, e);
    }
}

void LRUCache::LRU_Remove(LRUNode* e) {
    e->next->prev = e->prev;
    e->prev->next = e->next;
}

void LRUCache::LRU_Append(LRUNode* list, LRUNode* e) {
    e->next = list;
    e->prev = list->prev;
    e->prev->next = e;
    e->next->prev = e;
}

Cache::Handle* LRUCache::Lookup(const std::string& key, uint32_t hash) {
    std::lock_guard<std::mutex> lock(mutex_);
    LRUNode* e = table_.Lookup(key, hash);
    if (e != nullptr) Ref(e);
    return reinterpret_cast<Cache::Handle*>(e);
}

void LRUCache::Release(Cache::Handle* node) {
    std::lock_guard<std::mutex> lock(mutex_);
    Unref(reinterpret_cast<LRUNode*>(node));
}

Cache::Handle* LRUCache::Insert(const std::string& key, uint32_t hash, void* value,
                        size_t charge,
                        void (*deleter)(const std::string& key, void* value)) {
    std::lock_guard<std::mutex> lock(mutex_);
    LRUNode* e = reinterpret_cast<LRUNode*>(malloc(sizeof(LRUNode) - 1 + key.size()));
    e->value = value;
    e->deleter = deleter;
    e->charge = charge;
    e->key_length = key.size();
    e->hash = hash;
    e->in_cache = false;
    e->refs = 1;
    std::memcpy(e->key_data, key.data(), key.size());

    if (capacity_ > 0) {
        e->refs++;
        e->in_cache = true;
        LRU_Append(&in_usage_, e);
        usage_ += charge;
        FinishErase(table_.Insert(e));
    } else {
        e->next = nullptr;
    }
    while (usage_ > capacity_ && lru_.next != &lru_) {
        LRUNode * old = lru_.next;
        assert(old->refs == 1);
        bool erared = FinishErase(table_.Remove(old->key(), old->hash));
        if (!erared) {
            assert(erared);
        }
    }

    return reinterpret_cast<Cache::Handle*>(e);
}

bool LRUCache::FinishErase(LRUNode* e) {
    if (e != nullptr) {
        assert(e->in_cache);
        LRU_Remove(e);
        e->in_cache =false;
        usage_ -= e->charge;
        Unref(e);
    }
    return e != nullptr;
}

void LRUCache::Erase(const std::string& key, uint32_t hash) {
    std::lock_guard<std::mutex> lock(mutex_);
    FinishErase(table_.Remove(key, hash));
}

void LRUCache::Prune() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (lru_.next != &lru_) {
        LRUNode *e = lru_.next;
        assert(e->refs == 1);
        bool erased = FinishErase(table_.Remove(e->key(), e->hash));
        if (!erased) {
            assert(erased);
        }
    }
}