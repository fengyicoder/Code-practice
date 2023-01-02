#include <vector>
#include <memory>
#include <string.h>

#include "mem_pool.h"
#include "log.h"

using namespace std;

void print_total_size()
{
    int total_size = MemPool::getInstance().getTotalSizeKb();
    LOG_INFO("total size of mem pool is %dkb\n", total_size);
}

void print_left_size()
{
    int left_size = MemPool::getInstance().getLeftSizeKb();
    LOG_INFO("left size of mem pool is %dkb\n", left_size);
}

void print_list_size()
{
    int list_size;
    for(int index = mlow; index <= mUp; index = index * MEM_CAP_MULTI_POWER)
    {
        list_size = MemPool::getInstance().getListSizeByte((MEM_CAP)index);
        LOG_INFO("total size of pool's %dkb list is %dkb\n", index/1024, list_size/1024);
    }  
}

void print_list_content()
{
    for(int index = mlow; index <= mUp; index = index * MEM_CAP_MULTI_POWER)
    {
        MemPool::getInstance().printListContent((MEM_CAP)index);
    }  
}

void alloc_and_fill(shared_ptr<Chunk>& sp_chunk, vector<Chunk*>& chunks, MEM_CAP index, int num)
{
    for(int i=0; i<num; i++)
    {
        Chunk *b = MemPool::getInstance().allocChunk(index);
        b->copy(sp_chunk.get());
        chunks.push_back(b);
    }
}

void retrieve_chunks(vector<Chunk*>& chunks)
{
    for(auto i = chunks.begin(); i!=chunks.end(); ++i)
    {
        MemPool::getInstance().retrieve(*i);
    }
}

int main()
{
    Logger::getInstance()->init(NULL);

    LOG_INFO("===================before alloc...\n");
    print_total_size();
    print_left_size();
    print_list_size();
    print_list_content();

    vector<Chunk*> chunks;

    shared_ptr<Chunk> sp_chunk = make_shared<Chunk>(10);
    string s = "hello";
    strcpy(sp_chunk->data_, s.c_str());
    sp_chunk->length_ = strlen(s.c_str());

    LOG_INFO("===================start to alloc...\n");
    alloc_and_fill(sp_chunk, chunks, m4K, 1000);
    LOG_INFO("alloc 1000 chunks, chunk size: %dkb\n", m4K/1024);
    alloc_and_fill(sp_chunk, chunks, m16K, 1000);   
    LOG_INFO("alloc 1000 chunks, chunk size: %dkb\n", m16K/1024);
    alloc_and_fill(sp_chunk, chunks, m64K, 500);
    LOG_INFO("alloc 500 chunks, chunk size: %dkb\n", m64K/1024);
    alloc_and_fill(sp_chunk, chunks, m256K, 30);
    LOG_INFO("alloc 30 chunks, chunk size: %dkb\n", m256K/1024);
    alloc_and_fill(sp_chunk, chunks, m1M, 30);
    LOG_INFO("alloc 30 chunks, chunk size: %dkb\n", m1M/1024);
    alloc_and_fill(sp_chunk, chunks, m4M, 30);
    LOG_INFO("alloc 30 chunks, chunk size: %dkb\n", m4M/1024);

    LOG_INFO("===================after alloc...\n");
    print_total_size();
    print_left_size();
    print_list_size();
    print_list_content();

    LOG_INFO("===================start to retrieve...\n");
    retrieve_chunks(chunks);

    LOG_INFO("===================after retrieve...\n");
    print_total_size();
    print_left_size();
    print_list_size();
    print_list_content();

    return 0;
}