#include "ipc_workbase.h"
#include "fcntl.h"
#include "poll.h"

IIpcWorkBase::IIpcWorkBase(const std::string& name, IPC_WORK_TYPE work_type):
    pipe_path_(name), work_type_(work_type), bInited_(false) {}

IIpcWorkBase::~IIpcWorkBase() {}

bool IIpcWorkBase::CreateFile(std::string& path) {
    const char* cpath = path.c_str(); 
    if (!cpath || '\0' == path[0]) return false;
    char buf[256] = {0x0};
    size_t buf_len = sizeof(buf);
    snprintf(buf, buf_len, "%s", cpath);
    for (int i = 0; ++i; ) {
        char c = buf[i];
        if (c == '\\' || c == '/') {
            buf[i] = 0;
            struct stat sb;
            if (stat(buf, &sb) == 0 && S_ISDIR(sb.st_mode)) {}
            else {
                mkdir(buf, S_IRWXU);    
            }
            buf[i] = c;
        }
        if (c == 0) break;
    }
    int ret = mkfifo(cpath, 0600);
    return (ret == 0);
}

bool IIpcWorkBase::TryCreateFifo(std::string& path) {
    if (access(path.c_str(), F_OK) == -1) {
        bool ret = CreateFile(path);
        if (!ret) return false;
    }
    return true;
}

bool IIpcWorkBase::Init() {
    TryCreateFifo(pipe_path_);
    fd_ = open(pipe_path_.c_str(), (O_RDWR|O_NONBLOCK));
    if(fd_ == -1) return false;
    pipefds_[0] = pipefds_[1] = 0;
    if (-1 == pipe(pipefds_)) {
        assert(false);
        return false;
    }
    bInited_ = true;
    return true;
}

bool IIpcWorkBase::Uninit() {
    if (fd_ != 0) {
        close(fd_);
        fd_ = 0;
    }
    if (pipefds_[0]) close(pipefds_[0]);
    if (pipefds_[1]) close(pipefds_[1]);
    pipefds_[0] = pipefds_[1] = 0;
    unlink(pipe_path_.c_str());
    bInited_ = false;
}

void IIpcWorkBase::Loop() {
    if (bInited_ == false) return;
    for(;;) {
        int timeout(-1);
        pollfd fds[2];
    }
}

void IIpcWorkBase::Start() {
    thread_ = std::thread(Loop(), this);
}