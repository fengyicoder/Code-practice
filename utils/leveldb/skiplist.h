#pragma once

#include <atomic>
#include <cassert>
#include <cstdlib>

#include "mem.h"
#include "random.h"

template<typename Key, typename Comparactor>
class SkipList {
public:
    explicit SkipList(Comparactor cmp, Mem* mem);
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    void Insert(const Key& key);
    bool Contains(const Key& key) const;

    class Iterator {
    public:
        explicit Iterator(const SkipList* list);
        bool Valid() const;
        const Key& key() const;
        void Next();
        void Prev();
        void Seek(const Key& target);
        void SeekToFirst();
        void SeekToLast();

    private:
        const SkipList* list_;
        Node* node_;
    };

private:
    struct Node;
    enum { KMaxHeight = 12 };
    inline int GetMaxHeight() const;
    Node* NewNode(const Key& key, int height);
    int RandomHeight();
    bool Equal(const Key& a, const Key& b) const;
    bool KeyIsAfterNode(const Key& key, Node* n) const;
    Node* FindGreaterOrEqual(const Key& key, Node** prev) const;
    Node* FindLessThan(const Key& key) const;
    Node* FindLast() const;

private:
    Comparactor const compare_;
    Mem* const mem_;
    Node* const head_;
    std::atomic<int> max_height_;
    Random rnd_;
};

template<typename Key, typename Comparactor>
struct SkipList<Key, Comparactor>::Node {
    Key const key_;

    explicit Node(const Key& k): key_(k) {}

    Node* Next(int n) {
        assert(n >= 0);
        return next_[n].load(std::memory_order_acquire);
    }

    void SetNext(int n, Node* x) {
        assert(n >= 0);
        next_[n].store(x, std::memory_order_release);
    }

    Node* NoBarrier_Next(int n) {
        assert(n >= 0);
        return next_[n].load(std::memory_order_relaxed);
    }

    void NoBarrier_SetNext(int n, Node* x) {
        assert(n >= 0);
        next_[n].store(x, std::memory_order_relaxed);
    }

private:
    std::atomic<Node*> next_[1];
};

template<typename Key, typename Comparactor>
int SkipList<Key, Comparactor>::GetMaxHeight() const {
    return max_height_.load(std::memory_order_relaxed);
}

template<typename Key, typename Comparactor>
typename SkipList<Key, Comparactor>::Node* SkipList<Key, Comparactor>::NewNode(const Key& key, int height) {
    char* const node_memory = mem_->AllocateAligned(
        sizeof(Node) + sizeof(std::atomic<Node*>)*(height - 1));
    return new (node_memory) Node(key);
}

template<typename Key, typename Comparactor>
int SkipList<Key, Comparactor>::RandomHeight() {
    static const unsigned int kBranching = 4;
    int height = 1;
    while (height < KMaxHeight && rnd_.OneIn(kBranching)) {
        height++;
    }
    assert(height > 0);
    assert(height <= KMaxHeight)
    return height;
}

template<typename Key, typename Comparactor>
bool SkipList<Key, Comparactor>::Equal(const Key& a, const Key& b) const {
    return (compare_(a, b) == 0);
}

template<typename Key, typename Comparactor>
bool SkipList<Key, Comparactor>::KeyIsAfterNode(const Key& key, Node*n) const {
    return (n != nullptr) && (compare_(n->key, key) < 0);
}

template<typename Key, typename Comparactor>
typename SkipList<Key, Comparactor>::Node* 
SkipList<Key, Comparactor>::FindGreaterOrEqual(const Key& key, 
                                                Node** prev) const {
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    while (true) {
        Node* next = x->Next(level);
        if (KeyIsAfterNode(key, next)) {
            x = next;
        } else {
            if (prev) prev[level] = x;
            if (level == 0) return next;
            else level--;
        }
    }
}

template<typename Key, typename Comparactor>
typename SkipList<Key, Comparactor>::Node* 
SkipList<Key, Comparactor>::FindLessThan(const Key& key) const {
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    while (true) {
        assert(x == head_ || compare_(x->key, key) < 0);
        Node* next = x->Next(level);
        if (next == nullptr || compare_(next->key, key) >= 0) {
            if (level == 0) return x;
            else level--;
        } else {
            x = next;
        }
    }
}

template<typename Key, typename Comparactor>
typename SkipList<Key, Comparactor>::Node* 
SkipList<Key, Comparactor>::FindLast() const {
    Node* x = head_;
    int level = GetMaxHeight() - 1;
    while (true) {
        Node* next = x->Next(level);
        if (next == nullptr) {
            if (level == 0) return x;
            else level--;
        } else {
            x = next;
        }
    }
}

template<typename Key, typename Comparactor>
SkipList<Key, Comparactor>::SkipList(Comparactor cmp, Mem* mem): compare_(cmp),
      mem_(mem),
      head_(NewNode(0, KMaxHeight)),
      max_height_(1),
      rnd_(0xdeadbeef) {
    for (int i = 0; i < KMaxHeight; i++) {
        head_->setNext(i, nullptr);
    }
}

template<typename Key, typename Comparactor>
void SkipList<Key, Comparactor>::Insert(const Key& key) {
    Node* prev[KMaxHeight];
    Node* x = FindGreaterOrEqual(key, prev);
    assert(x == nullptr || !Equal(key, x->key));
    int height = RandomHeight();
    if (height > GetMaxHeight()) {
        for (int i = GetMaxHeight(); i < height; i++) {
            prev[i] = head_;
        }
        max_height_.store(height, std::memory_order_relaxed);
    }

    x = NewNode(key, height);
    for (int i = 0; i < height; i++) {
        x->NoBarrier_SetNext(i, prev[i]->NoBarrier_Next(i));
        prev[i]->SetNext(i, x);
    }
}

template<typename Key, typename Comparactor>
bool SkipList<Key, Comparactor>::Contains(const Key& key) const {
    Node* x = FindGreaterOrEqual(key, nullptr);
    if (x != nullptr && Equal(key, x->key)) {
        return true;
    } else {
        return false;
    }
}

template<typename Key, typename Comparactor>
inline SkipList<Key, Comparactor>::Iterator::Iterator(const SkipList* list): list_(list), node_(nullptr) {}

template<typename Key, typename Comparactor>
inline bool SkipList<Key, Comparactor>::Iterator::Valid() const {
    return node_ != nullptr;
}

template<typename Key, typename Comparactor>
inline const Key& SkipList<Key, Comparactor>::Iterator::key() const {
    assert(Valid())
    return node_->key;
}

template<typename Key, typename Comparactor>
inline void SkipList<Key, Comparactor>::Iterator::Next() {
    assert(Valid());
    node_ = node_->Next(0);
}

template<typename Key, typename Comparactor>
inline void SkipList<Key, Comparactor>::Iterator::Prev() {
    assert(Valid());
    node_ = list_->FindLessThan(node->key);
    if (node_ == list_->head_) node_ = nullptr;
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::Seek(const Key& target) {
  node_ = list_->FindGreaterOrEqual(target, nullptr);
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToFirst() {
  node_ = list_->head_->Next(0);
}

template <typename Key, class Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToLast() {
  node_ = list_->FindLast();
  if (node_ == list_->head_) {
    node_ = nullptr;
  }
}