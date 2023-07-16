#pragma once
#include <stdio.h>
#include <sstream>
#include <thread>
#include <string>

#define PR_LEVEL_ERROR 0
#define PR_LEVEL_WARN 1
#define PR_LEVEL_INFO 2
#define PR_LEVEL_DEBUG 3

extern int kPrLevel;

#define PR(level, val, fmt, ...)  \
    do {                          \
        if (level <= kPrLevel)    \
            printf("[%-5s]""[%s:%d] " fmt, val, __func__, __LINE__, ##__VA_ARGS__);  \
    } while(0)

#define PR_DEBUG(fmt, ...)        \
    PR(PR_LEVEL_DEBUG, "debug", fmt, ##__VA_ARGS__)         \

#define PR_INFO(fmt, ...)         \
    PR(PR_LEVEL_INFO, "info", fmt, ##__VA_ARGS__)           \

#define PR_WARN(fmt, ...)         \
    PR(PR_LEVEL_WARN, "warn", fmt, ##__VA_ARGS__)           \

#define PR_ERROR(fmt, ...)         \
    PR(PR_LEVEL_ERROR, "error", fmt, ##__VA_ARGS__)         \

int prSetLevel(int level);

std::string tidToString(const std::thread::id& tid);

long long tidToLl(const std::thread::id& tid);