#include <cstdio>
#include <thread>
#include <cassert>
#include <cstdlib>
#include <atomic>
#include "../pr.h"
#include "../log_queue.h"

const int KSize = 10;
typedef int qType;
// auto KQ(BufferQueue<qType>(KSize, true));
BufferQueue<qType> KQ(KSize, true);
std::atomic<int> KPCnt(0);
std::atomic<int> KCCnt(0);

void produer(int p_num) {
    long long tid = tidToLl(std::this_thread::get_id());
    PR_INFO("[tid:%lld] %d elements should be produced\n", tid, p_num);
    int i = 0;
    while (i < p_num) {
        bool ret = KQ.push(i);
        if (!ret) {
            PR_INFO("***[tid:%lld] push fail, buffer queue is full\n", tid);
        }
        else {
            PR_INFO("+++[tid:%lld] produced no.%d element\n", tid, i+1);
            KPCnt.fetch_add(1);
            i++;
        }
    }
}

void consumer(int c_num) {
    long long tid = tidToLl(std::this_thread::get_id());
    PR_INFO("[tid:%lld] %d elements should be consumed\n", tid, c_num);
    int temp;
    for (int i=0; i<c_num; i++) {
        KQ.pop(temp);
        PR_INFO("---[tid:%lld] consumed no.%d element\n", tid, i+1);
        KCCnt.fetch_add(1);
    }
}

void fillQ(BufferQueue<qType>& q, bool test=false) {
    for (int i=0; i<q.getCapacity(); i++) q.push(static_cast<qType>(i));
    if (test) {
        qType val;
        q.front(val);
        assert(val==0);
        for (int i=0; i<q.getCapacity(); i++) {
            q.pop(val);
            assert(val == i);
            if (i == q.getCapacity()-1) break;
            q.front(val);
            assert(val == i+1);
        }
    }
}

int main() {
    prSetLevel(PR_LEVEL_DEBUG);
    assert(KQ.isEmpty());
    assert(KQ.getCapacity() == KSize);
    assert(KQ.getRCnt() == 0);
    assert(KQ.getWCnt() == 0);

    fillQ(KQ);
    assert(KQ.isFull());
    assert(KQ.getRCnt()==0);
    assert(KQ.getWCnt()==KSize);

    KQ.clear();
    assert(KQ.isEmpty());
    assert(KQ.getRCnt() == 0);
    assert(KQ.getWCnt() == 0);

    fillQ(KQ, true);
    assert(KQ.getRCnt() == KSize);
    assert(KQ.getWCnt() == KSize);
    assert(KQ.isEmpty());
    fillQ(KQ);
    assert(KQ.getRCnt() == KSize);
    assert(KQ.getWCnt() == KSize*2);

    KQ.clear();

    int temp;
    auto start = std::chrono::steady_clock::now();
    bool ret = KQ.pop(temp, 2000);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    PR_INFO("cond wait_for duration: %d ms\n" ,static_cast<int>(duration.count()));
    assert(!ret);

    PR_INFO("buffer queue function test passed!\n\n\n");

    KQ.clear();
    fillQ(KQ);
    int origin_num = KQ.getSize();
    assert(origin_num==KSize);
    PR_INFO("------------------------------------------\n");
    PR_INFO("buffer queue capacity is %d\n", KSize);
    PR_INFO("origin element num is %d\n", origin_num);
    PR_INFO("------------------------------------------\n");
    int p_n1 = 100;
    int p_n2 = 50;
    int c_n1 = 160;
    std::thread p1(produer, p_n1);
    std::thread p2(produer, p_n2);
    std::thread c1(consumer, c_n1);
 
    p1.join();
    p2.join();
    c1.join();

    PR_INFO("\n");
    PR_INFO("origin element num is %d\n", origin_num);
    PR_INFO("totally produced %d elements\n", KPCnt.load());
    PR_INFO("totally consumed %d elements\n", KCCnt.load());

    assert( KQ.getSize() == ( p_n1 + p_n2 + KSize - c_n1 ) );
    assert( KQ.getRCnt() == ( c_n1 ) );
    assert( KQ.getWCnt() == ( KSize + p_n1 + p_n2 ) );
    PR_INFO("buffer queue mutithread test passed!\n");

    return 0;
}