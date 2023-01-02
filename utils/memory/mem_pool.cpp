#include <cassert>

#include "../log/pr.h"
#include "mem_pool.h"

void MemPool::memInit(MEM_CAP size, int chunk_num) {
    Chunk *prev;
    mp_pool_[size] = new (std::nothrow) Chunk(size);
    if (mp_pool_[size] == nullptr) {
        PR_ERROR("new chunk %d error", static_cast<int>(size));
        exit(1);
    }
    prev = mp_pool_[size];
    for (int i=1; i<chunk_num; i++) {
        prev->next_ = new (std::nothrow)Chunk(size);
        if (prev->next_ == nullptr) {
            PR_ERROR("new chunk m4K error");
            exit(1);
        }
        prev = prev->next_;
    }
    mp_total_size_kb_ += size/1024 * chunk_num;
}

MemPool::MemPool(): mp_total_size_kb_(0), mp_left_size_kb_(0) {
    memInit(m4K, 2000);
    memInit(m16K, 500);
    memInit(m64K, 250);
    memInit(m256K, 100);
    memInit(m1M, 25);
    memInit(m4M, 10);
    mp_left_size_kb_ = mp_total_size_kb_;
}

Chunk* MemPool::allocChunk(int n) {
    int index;
    bool found = false;
    for (index = mlow; index <= mUp; index = index*MEM_CAP_MULTI_POWER) {
        if (n <= index) {
            found = true;
            break;
        }
    }
    if (!found) return nullptr;
    std::lock_guard<std::mutex> lock(mp_mutex_);
    if (mp_pool_[index] == nullptr) {
        if (mp_total_size_kb_ + index/1024 >= MAX_POOL_SIZE) {
            PR_ERROR("beyond the limit size of memory!\n");
            exit(1);
        }
        Chunk* new_buf = new (std::nothrow) Chunk(index);
        if (new_buf == nullptr) {
            PR_ERROR("new chunk error\n");
            exit(1);
        }
        mp_total_size_kb_ += index/1024;
        return new_buf;
    }
    Chunk *target = mp_pool_[index];
    mp_pool_[index] = target->next_;
    target->next_ = nullptr;
    mp_left_size_kb_ -= index/1024;
    return target;
}

void MemPool::retrieve(Chunk* block) {
    int index = block->capacity_;
    block->head_ = 0;
    block->length_ = 0;
    std::lock_guard<std::mutex> lock(mp_mutex_);
    assert(mp_pool_.find(index) != mp_pool_.end());
    block->next_ = mp_pool_[index];
    mp_pool_[index] = block;
    mp_left_size_kb_ += block->capacity_/1024;
}

int MemPool::getListSizeByte(MEM_CAP index) {
    int size = 0;
    std::lock_guard<std::mutex> lock(mp_mutex_);
    assert(mp_pool_.find(index) != mp_pool_.end());
    Chunk *node = mp_pool_[index];
    while (node)
    {
        size += node->capacity_;
        node = node->next_;
    }
    return size;
}

void MemPool::printListContent(MEM_CAP index) {
    std::lock_guard<std::mutex> lock(mp_mutex_);
    int cnt = 0;
    printf("***************start to print %dkb chunk_size list data*******************\n", index/1024);
    assert(mp_pool_.find(index) != mp_pool_.end());
    Chunk *node = mp_pool_[index];
    while (node)
    {
        if (cnt <= 5) node->printData();
        cnt++;
        node = node->next_;
    }
    printf("...\n");
    printf("******************end, node cnt is %d************************\n\n", cnt);
}