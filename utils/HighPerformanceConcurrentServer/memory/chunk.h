#pragma once

struct Chunk {
    explicit Chunk(int size);
    ~Chunk();
    void clear();
    void adjust();
    void copy(const Chunk* other);
    void pop(int len);

    [[deprecated("chunk debug api deprecated!")]]
    void printData();

    int capacity_{0};
    int length_{0};
    int head_{0};
    char* data_{nullptr};
    Chunk* next_{nullptr};
};