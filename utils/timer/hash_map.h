#pragma once

#include <unordered_map>
#include <mutex>

template<typename K, typename V>
class HashMap {
public:
    void emplace(const K& key, const V& v) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_[key] = v;
    }
    void emplace(const K& key, V&& v) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_[key] = std::move(v);
    }
    void erase(const K& key) {
        std::lock_guard<std::mutex> lock(mutex_);
        if(map_.find(key) != map_.end()) map_.erase(key);
    }
    bool getVal(const K& key, V& value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (map_.find(key) != map_.end()) {
            value = map_[key];
            return true;
        }
        return false;
    }
    bool isKeyExist(const K& key) {
       std::lock_guard<std::mutex> lock(mutex_);
       return map_.find(key) != map_.end(); 
    }
    size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.size();
    }

private:
    std::unordered_map<K, V> map_;
    std::mutex mutex_;
};