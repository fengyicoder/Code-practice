#include <ctime>
#include <chrono>
#include <stdexcept>
#include "log.h"


Logger::log_level KLogLevel = Logger::LOG_LEVEL_INFO;

const char *LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "[ERROR]",
    "[WARN ]",
    "[INFO ]",
    "[DEBUG]",
};

struct my_time {
    int year;
    char month;
    char day;
    char hour;
    char minute;
    char second;
};

static my_time getCurrentSysTime() {
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm* ptm = localtime(&tt);
    my_time t = {ptm->tm_year+1900, static_cast<char>(ptm->tm_mon+1), static_cast<char>(ptm->tm_mday),
                static_cast<char>(ptm->tm_hour), static_cast<char>(ptm->tm_min), static_cast<char>(ptm->tm_sec)};
    return t;
}

Logger::Logger() {
    PR_DEBUG("log class constructed\n");
    count_ = 0;
}

Logger::~Logger() {
    if (asyncw_thread_) {
        is_thread_stop_ = true;
        if (asyncw_thread_->joinable()) {
            buffer_queue_->notify();
            asyncw_thread_->join();
        }
        delete asyncw_thread_;
    }
    std::lock_guard<std::mutex> lock(mtx_);
    if (fp_ != NULL) {
        fclose(fp_);
    }
    if (buf_) {
        delete [] buf_;
    }
    if (buffer_queue_) {
        delete buffer_queue_;
    }
}

bool Logger::init(const char* file_name, int buffer_queue_size, log_level level,
                int buffer_size, int split_lines) {
    if (is_inited_) {
        PR_WARN("Logger has been initialized, do not try again!\n");
        return false;
    }
    if (!file_name) is_stdout_ = true;
    if (!is_stdout_ && strlen(file_name)>=128) {
        PR_WARN("file name must be less than 128 bytes\n");
        throw std::invalid_argument("file name must be less than 128 bytes\n");
    }
    setLogLevel(level);
    if (buffer_queue_size >= 1) {
        buffer_queue_ = new BufferQueue<std::string>(buffer_queue_size);
        is_async_ = true;
        asyncw_thread_ = new std::thread(&Logger::asyncFlush);
    }
    buf_size_ = buffer_size;
    buf_ = new char[buf_size_];
    memset(buf_, '\0', buf_size_);
    split_lines_ = split_lines;
    my_time tm = getCurrentSysTime();
    today_ = tm.day;

    if (is_stdout_) {
        is_inited_ = true;
        fp_ = stdout;
        PR_DEBUG("succeed in using stdout as log output\n");
        PR_DEBUG("log init finished!\n");
        return true;
    }

    const char *p = strrchr(file_name, '/');
    char log_file_fullname[268] = {0};
    if (p == NULL) {
        PR_ERROR("log file name should behind '/'\n");
        return false;
    }
    else {
        strcpy(file_name_, p+1);
        strncpy(dir_name_, file_name, p-file_name+1);
        snprintf(log_file_fullname, 267, "%s%04d_%02d_%02d_%s", dir_name_, tm.year, tm.month, tm.day, file_name_);
        fp_ = fopen(log_file_fullname, "a");
    }
    if (fp_ == NULL) {
        PR_ERROR("open %s failed!\n", log_file_fullname);
        return false;
    }
    is_inited_ = true;
    PR_DEBUG("succeed in using file %s as log output\n", log_file_fullname);
    PR_DEBUG("log init finished!\n");

    return true;
}

void Logger::writeLog(const char* file_name, const char* callback_name, int line_no, log_level level, const char* format, ...) {
    my_time my_tm = getCurrentSysTime();
    {
        std::lock_guard<std::mutex> lock(mtx_);
        count_++;
        if (today_ != my_tm.day || count_ % split_lines_ == 0) {
            PR_DEBUG("start to create a new log file\n");
            char new_file_name[301] = {0};
            fflush(fp_);
            fclose(fp_);
            char prefix[24] = {0};
            snprintf(prefix, 23, "%04d_%02d_%02d_", my_tm.year, my_tm.month, my_tm.day);
            if (today_ != my_tm.day) {
                snprintf(new_file_name, 300, "%s%s%s", dir_name_, prefix, file_name_);
                today_ = my_tm.day;
                count_ = 0;
            }
            else {
                snprintf(new_file_name, 300, "%s%s%s.%lld", dir_name_, prefix, file_name_, count_ / split_lines_);
            }
            fp_ = fopen(new_file_name, "a");
        }
    }

    va_list valist;
    va_start(valist, format);
    std::string log_str;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        int n = snprintf(buf_, 300, "%04d-%02d-%02d %02d:%02d:%02d %s [%s:%s:%d] ",
                        my_tm.year, my_tm.month, my_tm.day,
                            my_tm.hour, my_tm.minute, my_tm.second, LogLevelName[level],
                            file_name, callback_name, line_no);
        int m = vsnprintf(buf_+n, buf_size_-1, format, valist);
        buf_[n+m] = '\n';
        buf_[n+m+1] = '\0';
        log_str = buf_;
    }
    va_end(valist);
    if (is_async_) {
        while (!buffer_queue_->push(log_str) && !is_thread_stop_) {}
    }
    else {
        std::lock_guard<std::mutex> lock(mtx_);
        fputs(log_str.c_str(), fp_);
    }
}

void Logger::flush(void)
{
    std::lock_guard<std::mutex> lock(mtx_);
    fflush(fp_);
}