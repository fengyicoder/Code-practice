#include <cassert>
#include <memory.h>
#include <cstdio>
#include <cstring>
#include "chunk.h"

Chunk::Chunk(int size): capacity_(size), data_(new char[size]) {
    assert(data_);
}

Chunk::~Chunk() {
    if (data_) delete [] data_;
}

void Chunk::clear() {
    length_ = head_ = 0;
}

void Chunk::adjust() {
    if (head_ != 0) {
        if (length_ != 0) {
            memmove(data_, data_+head_, length_);
        }
        head_ = 0;
    }
}

void Chunk::copy(const Chunk* other) {
    memcpy(data_, other->data_+other->head_, other->length_);
    head_ = 0;
    length_ = other->length_;
}

void Chunk::pop(int len) {
    length_ -= len;
    head_ += len;
}

void Chunk::printData()
{
    for(int i=head_; i<length_; i++)
    {
        printf("%c", data_[i]);
    }
    printf("\n");
}
