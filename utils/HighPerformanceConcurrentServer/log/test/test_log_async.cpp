#include <chrono>
#include <thread>
#include <vector>
#include <atomic>

#include "../log.h"
#include "../pr.h"

const int KItemNum = 11000;
const int KTNum = 5;
std::atomic<int> KTCnt{0};

void logAsyncTest() {
    long long tid = tidToLl(std::this_thread::get_id());
    PR_INFO("[threa_%d]tid is: %lld\n", KTCnt.fetch_add(1), tid);

    for(int i=0; i<KItemNum; i++)
    {
        LOG_INFO("[tid:%lld] log async test: %d\n", tid, i);
    }
}

int main() {
    Logger::getInstance()->init("./log_async.txt", 10);
    std::vector<std::thread> threads;

    PR_INFO("async log test started\n");
    PR_INFO("start to log\n");
    auto start = std::chrono::steady_clock::now();
    for (int i=0; i<KTNum; i++) {
        threads.emplace_back(logAsyncTest);
    }
    for (int i=0; i<KTNum; i++) {
        threads[i].join();
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    PR_INFO("end logging\n"); 
    PR_INFO("totally write %d items into files\n", KItemNum * KTNum);
    PR_INFO("costed time: %d ms\n" ,static_cast<int>(duration.count()));
    return 0;
}