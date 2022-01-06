#include <iostream>
#include <unordered_map>

struct Node
{
    int key, val;
    Node *next, *prev;
    Node(int k, int v)
    {
        key = k;
        val = v;
    }
};

struct DoubleList
{
    // 头尾虚节点
    Node *head, *tail;
    // 链表元素数
    int size;

    DoubleList()
    {
        // 初始化双向链表的数据
        head = new Node(0, 0);
        tail = new Node(0, 0);
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    //尾部添加节点
    void addLast(Node *x)
    {
        x->prev = tail->prev;
        x->next = tail;
        tail->prev->next = x;
        tail->prev = x;
        size++;
    }

    //删除节点
    void remove(Node *x)
    {
        x->prev->next = x->next;
        x->next->prev = x->prev;
        size--;
    }

    //删除第一个节点
    Node *removeFirst()
    {
        if (head->next == tail)
            return nullptr;
        Node *first = head->next;
        remove(head->next);
        return first;
    }

    //返回链表长度
    int size() { return size; }
};

struct LRUCache
{
    std::unordered_map<int, Node *> map;
    DoubleList cache;
    int cap;
    LRUCache(int capacity)
    {
        cap = capacity;
    }

    void makeRecently(int key)
    {
        Node *x = map[key];
        cache.remove(x);
        cache.addLast(x);
    }

    void addRencently(int key, int val)
    {
        Node *x = new Node(key, val);
        cache.addLast(x);
        map.insert(std::make_pair(key, x));
    }

    void deleteKey(int key)
    {
        Node *x = map.at(key);
        cache.remove(x);
        map.erase(key);
    }

    void removeLeastRecently()
    {
        Node *x = cache.removeFirst();
        int deleteKey = x->key;
        map.erase(deleteKey);
    }

    int get(int key)
    {
        if (map.count(key) == 0)
            return -1;
        makeRecently(key);
        return map.at(key)->val;
    }

    void put(int key, int val)
    {
        if (map.count(key))
        {
            deleteKey(key);
            addRencently(key, val);
        }

        if (cap == cache.size)
        {
            removeLeastRecently();
        }
        addRencently(key, val);
    }
};
