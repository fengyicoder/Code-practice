#pragma once

#include <unordered_map>
#include <mutex>

#include "chunk.h"

typedef std::unordered_map<int, Chunk*> pool_t;

#define MEM_CAP_MULTI_POWER (4)

typedef enum {
    mlow = 4096,
    m4K = mlow,
    m16K = m4K * MEM_CAP_MULTI_POWER,
    m64K = m16K* MEM_CAP_MULTI_POWER,
    m256K = m64K * MEM_CAP_MULTI_POWER,
    m1M = m256K * MEM_CAP_MULTI_POWER,
    m4M = m1M * MEM_CAP_MULTI_POWER,
    mUp = m4M
} MEM_CAP;

#define MAX_POOL_SIZE (4U * 1024 * 1024)

class MemPool {
public:
    static MemPool& getInstance() {
        static MemPool mp_instance;
        return mp_instance;
    }

    Chunk* allocChunk(int n);
    Chunk* allocChunk() { return allocChunk(m4K); }
    void retrieve(Chunk* block);
    ~MemPool() = default;
    [[deprecated("mem pool debug api deprecated!")]]
    int getTotalSizeKb() { return mp_total_size_kb_; }
    [[deprecated("mem pool debug api deprecated!")]]
    int getLeftSizeKb() { return mp_left_size_kb_; }
    [[deprecated("mem pool debug api deprecated!")]]
    int getListSizeByte(MEM_CAP index);
    [[deprecated("mem pool debug api deprecated!")]]
    void printListContent(MEM_CAP index);
private:
    MemPool();
    MemPool(const MemPool&) = delete;
    MemPool(MemPool&&) = delete;
    MemPool& operator=(const MemPool&) = delete;
    MemPool& operator=(MemPool&&) = delete;
    void memInit(MEM_CAP size, int chunk_num);

private:
    pool_t mp_pool_;
    uint64_t mp_total_size_kb_;
    uint64_t mp_left_size_kb_;
    std::mutex mp_mutex_;
};