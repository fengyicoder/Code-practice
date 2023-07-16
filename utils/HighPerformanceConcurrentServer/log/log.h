#pragma once

#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cstring>
#include <thread>
#include <cassert>
#include <atomic>

#include "log_queue.h"
#include "pr.h"

class Logger {
public:
    typedef enum {
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG,
        NUM_LOG_LEVELS,
    } log_level;

    ~Logger();
    static Logger* getInstance() {
        static Logger instance;
        return &instance;
    }

    static void asyncFlush() {
        Logger::getInstance()->asyncWrite();
    }

    static log_level getLogLevel();
    static log_level setLogLevel(log_level level);
    bool init(const char* file_name, int buffer_queue_size=0, log_level level = Logger::LOG_LEVEL_INFO,
                int buffer_size = 8192, int split_lines = 5000);
    bool isInited() {
        return is_inited_;
    }
    void writeLog(const char* file_name, const char* callback_name, int line_no, log_level level, const char* format, ...);
    void flush();


private:
    Logger();
    Logger(const Logger&);
    void* asyncWrite() {
        std::string single_line;
        while(buffer_queue_->pop(single_line) && !is_thread_stop_) {
            std::lock_guard<std::mutex> lock(mtx_);
            fputs(single_line.c_str(), fp_);
        }
    }


private:
    char dir_name_[128];
    char file_name_[128];
    int split_lines_;
    int buf_size_;
    long long count_;
    int today_;
    FILE* fp_;
    char* buf_ = nullptr;
    bool is_inited_ = false;
    BufferQueue<std::string> *buffer_queue_ = nullptr;
    bool is_async_;
    bool is_stdout_ = false;
    std::atomic<bool> is_thread_stop_{false};
    std::mutex mtx_;
    std::thread *asyncw_thread_;
};

extern Logger::log_level KLogLevel;

inline Logger::log_level Logger::getLogLevel() {
    return KLogLevel;
}

inline Logger::log_level Logger::setLogLevel(Logger::log_level level) {
    assert(level >= Logger::LOG_LEVEL_ERROR && level <= Logger::LOG_LEVEL_DEBUG);
    Logger::log_level old_level = KLogLevel;
    KLogLevel = level;
    return old_level;
}

#define LOG_DEBUG(format, ...) \
    do {                                           \
        if (!Logger::getInstance()->isInited())   \
            PR_ERROR("logger must be inited before user!\n"); \
        if (Logger::LOG_LEVEL_DEBUG <= Logger::getLogLevel()) \
        {                                                     \
            Logger::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, Logger::LOG_LEVEL_DEBUG, format, ##__VA_ARGS__); \
            Logger::getInstance()->flush();                    \
        }                                                      \
    } while (0)

#define LOG_INFO(format, ...) \
    do {                                           \
        if (!Logger::getInstance()->isInited())   \
            PR_ERROR("logger must be inited before user!\n"); \
        if (Logger::LOG_LEVEL_INFO <= Logger::getLogLevel()) \
        {                                                     \
            Logger::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, Logger::LOG_LEVEL_INFO, format, ##__VA_ARGS__); \
            Logger::getInstance()->flush();                    \
        }                                                      \
    } while (0)

#define LOG_WARN(format, ...) \
    do {                                           \
        if (!Logger::getInstance()->isInited())   \
            PR_ERROR("logger must be inited before user!\n"); \
        if (Logger::LOG_LEVEL_WARN <= Logger::getLogLevel()) \
        {                                                     \
            Logger::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, Logger::LOG_LEVEL_WARN, format, ##__VA_ARGS__); \
            Logger::getInstance()->flush();                    \
        }                                                      \
    } while (0)

#define LOG_ERROR(format, ...) \
    do {                                           \
        if (!Logger::getInstance()->isInited())   \
            PR_ERROR("logger must be inited before user!\n"); \
        if (Logger::LOG_LEVEL_ERROR <= Logger::getLogLevel()) \
        {                                                     \
            Logger::getInstance()->writeLog(__FILE__, __FUNCTION__, __LINE__, Logger::LOG_LEVEL_ERROR, format, ##__VA_ARGS__); \
            Logger::getInstance()->flush();                    \
        }                                                      \
    } while (0)                                                         \
