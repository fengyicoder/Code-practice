#include <unordered_map>

using namespace std;

struct LinkedHashSet
{
    LinkedHashSet(int);
};

struct LFUCache
{
    // key 到 val 的映射，我们后文称为 KV 表
    unordered_map<int, int> keyToVal;
    // key 到 freq 的映射，我们后文称为 KF 表
    unordered_map<int, int> keyToFreq;
    // freq 到 key 列表的映射，我们后文称为 FK 表
    unordered_map<int, LinkedHashSet<int>> freqToKeys;
    // 记录最小的频次
    int minFreq;
    // 记录 LFU 缓存的最大容量
    int cap;

    LFUCache(int capacity)
    {
        cap = capacity;
        minFreq = 0;
    }

    int get(int key) {
        if (keyToVal.count(key) == 0) return -1;
        increaseFreq(key);
        return keyToVal[key];
    }

    void put(int key, int val) {}
};
