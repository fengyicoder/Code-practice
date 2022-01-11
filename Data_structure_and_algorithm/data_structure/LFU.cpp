#include <unordered_map>
#include <list>

using namespace std;

#ifdef KV
template <class KeyType, class MappedType,
            class Hash = std::hash<KeyType>,
            class Pred = std::equal_to<KeyType> >
class LinkedHashSet
{
public:
    typedef std::pair<const KeyType, MappedType> EntryType;
    typedef typename std::list<EntryType>::iterator iterator;
    typedef typename std::list<EntryType>::const_iterator const_iterator;

private:
    typedef std::unordered_map<KeyType, iterator, Hash, Pred> HashMap;
    typedef typename HashMap::iterator _map_itr;
    typedef typename HashMap::const_iterator _map_citr;
    size_t _size;
    std::list<EntryType> _values;
    HashMap _map_key2entry;

public:
    std::pair<iterator, bool> insert(const EntryType& new_entry) {
        _map_itr pIt = _map_key2entry.find(new_entry.first);
        if (pIt == _map_key2entry.end()) {
            _values.push_back(new_entry);
            _map_key2entry.insert(std::pair<KeyType, iterator>(new_entry.first, --_ _values.end()));
            ++_size;
        }
        else {
            _values.erase(pIt->second);
            _values.push_back(new_entry);
            pIt->second = --_values.end();
        }
        return std::pair<iterator, bool>(--_values.end(), true);
    }

    iterator insert(const_iterator it, const EntryType& new_entry) {
        _map_itr pIt = _map_key2entry.find(new_entry.first);
        if(pIt != _map_key2entry.end()) {
            _values.erase(pIt->second);
            --_size;
        }
        iterator _it = _values.insert(it, new_entry);
        _map_key2entry.insert(std::pair<KeyType, iterator>(new_entry.first, _it));
        ++_size;
        return _it;
    }

    inline iterator begin() { return _values.begin();}
    inline const_iterator begin() const { return _values.begin();}
    inline iterator end() { return _values.end();}
    inline const_iterator end() const { return _values.end();}
    
    iterator find(const KeyType& key) {
        if (_size > 0) {
            _map_itr it = _map_key2entry.find(key);
            if (it == _map_key2entry.end()) return _map_key2entry.end();
            iterator find_it = it.second;
            return find_it;
        }
        return _values.end();
    }

    const_iterator find(const KeyType& key) const {
        if (_size > 0) {
            _map_citr it = _map_key2entry.find(key);
            if (it == _map_key2entry.end()) {
                return _values.end();
            }
            const_iterator find_it = it->second;
            return find_it;
        }
        return _values.end();
    }

    void erase(iterator pos) {
        assert(pos != _values.end());
        assert(_size != 0);

        EntryType v = *pos;
        _values.erase(pos);
        _map_key2entry.erase(v.first);
        --_size;
    }   

    void erase(const KeyType& key) {
        iterator it = find(key);
        if (it != _values.end()) {
            erase(it);
        }

    }

    void clear() {
        _values.clear();
        _map_key2entry.clear();
        _size = 0;
    }

    inline size_t size() {
        return _size;
    }

    inline bool empty() const {
        return _values.empty();
    }

};

#else

template <class KeyType,
            class Hash = std::hash<KeyType>,
            class Pred = std::equal_to<KeyType> >
class LinkedHashSet
{
public:
    typedef KeyType EntryType;
    typedef typename std::list<EntryType>::iterator iterator;
    typedef typename std::list<EntryType>::const_iterator const_iterator;

private:
    typedef std::unordered_map<KeyType, iterator, Hash, Pred> HashMap;
    typedef typename HashMap::iterator _map_itr;
    typedef typename HashMap::const_iterator _map_citr;
    size_t _size;
    std::list<EntryType> _values;
    HashMap _map_key2entry;

public:
    std::pair<iterator, bool> insert(const EntryType& new_entry) {
        _map_itr pIt = _map_key2entry.find(new_entry);
        if (pIt == _map_key2entry.end()) {
            _values.push_back(new_entry);
            _map_key2entry.insert(std::pair<KeyType, iterator>(new_entry, --_ _values.end()));
            ++_size;
        }
        else {
            _values.erase(pIt->second);
            _values.push_back(new_entry);
            pIt->second = --_values.end();
        }
        return std::pair<iterator, bool>(--_values.end(), true);
    }

    iterator insert(const_iterator it, const EntryType& new_entry) {
        _map_itr pIt = _map_key2entry.find(new_entry);
        if(pIt != _map_key2entry.end()) {
            _values.erase(pIt->second);
            --_size;
        }
        iterator _it = _values.insert(it, new_entry);
        _map_key2entry.insert(std::pair<KeyType, iterator>(new_entry, _it));
        ++_size;
        return _it;
    }

    inline iterator begin() { return _values.begin();}
    inline const_iterator begin() const { return _values.begin();}
    inline iterator end() { return _values.end();}
    inline const_iterator end() const { return _values.end();}
    
    iterator find(const KeyType& key) {
        if (_size > 0) {
            _map_itr it = _map_key2entry.find(key);
            if (it == _map_key2entry.end()) return _map_key2entry.end();
            iterator find_it = it.second;
            return find_it;
        }
        return _values.end();
    }

    const_iterator find(const KeyType& key) const {
        if (_size > 0) {
            _map_citr it = _map_key2entry.find(key);
            if (it == _map_key2entry.end()) {
                return _values.end();
            }
            const_iterator find_it = it->second;
            return find_it;
        }
        return _values.end();
    }

    void erase(iterator pos) {
        assert(pos != _values.end());
        assert(_size != 0);

        EntryType v = *pos;
        _values.erase(pos);
        _map_key2entry.erase(v);
        --_size;
    }   

    void erase(const KeyType& key) {
        iterator it = find(key);
        if (it != _values.end()) {
            erase(it);
        }

    }

    void clear() {
        _values.clear();
        _map_key2entry.clear();
        _size = 0;
    }

    inline size_t size() {
        return _size;
    }

    inline bool empty() const {
        return _values.empty();
    }
};

#endif

struct LFUCache
{
    // key 到 val 的映射，我们后文称为 KV 表
    unordered_map<int, int> keyToVal;
    // key 到 freq 的映射，我们后文称为 KF 表
    unordered_map<int, int> keyToFreq;
    // freq 到 key 列表的映射，我们后文称为 FK 表
    unordered_map<int, LinkedHashSet<int> > freqToKeys;
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

    void put(int key, int val) {
        if (cap <= 0) return;
        if (keyToVal.count(key)) {
            keyToVal[key] = val;
            increaseFreq(key);
        }

        if (cap <= keyToVal.size()) {
            removeMinFreqKey();
        }
        keyToVal.insert(std::make_pair(key, val));
        keyToFreq.insert(std::make_pair(key, 1));
        freqToKeys.insert(std::make_pair(1, LinkedHashSet<int>()));
        freqToKeys[1].insert(key);
        minFreq = 1;
    }

    void increaseFreq(int key) {
        int freq = keyToFreq[key];
        keyToFreq[key] = freq + 1;
        freqToKeys[freq].erase(key);
        freqToKeys.insert(std::make_pair(freq+1, LinkedHashSet<int>()));
        freqToKeys[freq+1].insert(key);
        if (freqToKeys[freq].empty()) {
            freqToKeys.erase(freq);
            if (freq == minFreq) minFreq++;
        }
    }

    void removeMinFreqKey() {
        LinkedHashSet<int> keyList = freqToKeys[minFreq];
        int deleteKey = *keyList.begin();
        keyList.erase(deleteKey);
        if (keyList.empty()) {
            freqToKeys.erase(minFreq);
        }
        keyToFreq.erase(deleteKey);
        keyToVal.erase(deleteKey);
    }
};
