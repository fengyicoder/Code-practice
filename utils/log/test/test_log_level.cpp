#include "../log.h"
#include "../pr.h"

void logLevelTest() {
    PR_INFO("\n------------------set log level as debug----------------\n");
    Logger::setLogLevel(Logger::LOG_LEVEL_DEBUG);
    LOG_ERROR("*****************log error\n");
    LOG_WARN("*****************log warn\n");
    LOG_INFO("*****************log info\n");
    LOG_DEBUG("*****************log debug\n");

    PR_INFO("\n------------------set log level as info----------------\n");
    Logger::setLogLevel(Logger::LOG_LEVEL_INFO);
    LOG_ERROR("*****************log error\n");
    LOG_WARN("*****************log warn\n");
    LOG_INFO("*****************log info\n");
    LOG_DEBUG("*****************log debug\n");

    PR_INFO("\n------------------set log level as warn----------------\n");
    Logger::setLogLevel(Logger::LOG_LEVEL_WARN);
    LOG_ERROR("*****************log error\n");
    LOG_WARN("*****************log warn\n");
    LOG_INFO("*****************log info\n");
    LOG_DEBUG("*****************log debug\n");

    PR_INFO("\n------------------set log level as error----------------\n");
    Logger::setLogLevel(Logger::LOG_LEVEL_ERROR);
    LOG_ERROR("*****************log error\n");
    LOG_WARN("*****************log warn\n");
    LOG_INFO("*****************log info\n");
    LOG_DEBUG("*****************log debug\n");
}

int main() {
    Logger::getInstance()->init(NULL);
    logLevelTest();
}