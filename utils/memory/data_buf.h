#pragma once

#include "chunk.h"
#include "mem_pool.h"

class BufferBase {
public:
    BufferBase();
    ~BufferBase();

    const int length() const;
    void pop(int len);
    void clear();

protected:
    Chunk *data_buf_{nullptr};
};

class InputBuffer: public BufferBase
{
public:
    int readFromFd(int fd);
    const char* getFromBuf() const;
    void adjust();
};

class OutputBuffer: public BufferBase
{
public:
    int write2buf(const char* data, int len);
    int write2fd(int fd);
};