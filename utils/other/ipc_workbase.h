#pragma once

#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <queue>
#include <string>
#include <thread>

enum class IPC_WORK_TYPE {
    IPC_WORK_TYPE_READ = 0,
    IPC_WORK_TYPE_WRITE,
    IPC_WORK_TYPE_COUNT,
    IPC_WORK_TYPE_INVALID
};

class IIpcWorkBase {
public:
    IIpcWorkBase(const std::string& name, IPC_WORK_TYPE work_type);
    virtual ~IIpcWorkBase();
    virtual bool Init();
    void Uninit();
    void Stop();
    void Start();
    void Loop();

protected:
    virtual bool ProcessData() {};
    virtual bool HasDataToWrite() {};

private:
    bool CreateFile(std::string& path);
    bool TryCreateFifo(std::string& path);

private:
    IPC_WORK_TYPE work_type_;
    std::string pipe_path_;
    std::thread thread_;
    int fd_;
    int pipefds_[2];
    bool bInited_;
};