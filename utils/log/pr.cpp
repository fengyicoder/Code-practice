#include "pr.h"

int kPrLevel = PR_LEVEL_INFO;

int prSetLevel(int level) {
    if (level > PR_LEVEL_DEBUG) level = PR_LEVEL_DEBUG;
    else if (level < PR_LEVEL_ERROR) level = PR_LEVEL_ERROR;
    int oldLevel = kPrLevel;
    kPrLevel = level;
    return oldLevel;
}

std::string tidToString(const std::thread::id& tid) {
    std::ostringstream oss;
    oss << tid << std::endl;
    return oss.str();
}

long long tidToLl(const std::thread::id& tid) {
    return std::atoll(tidToString(tid).c_str());
}