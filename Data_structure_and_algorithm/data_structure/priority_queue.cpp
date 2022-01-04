#include <iostream>

template<typename Key>
class MaxPQ
{
public:
    MaxPQ(int cap) {
        pq = new Key[cap + 1];
    }
    ~MaxPQ() = default;

    Key max() {
        return pq[1];
    }

    void insert(Key e) {
        N++;
        pq[N] = e;
        swim(N);
    }

    Key delMax() {
        Key max = pq[1];
        exch(1, N);
        pq[N] = NULL;
        N--;
        sink(1);
        return max;
    }

private:
    int parent(int root) {
        return root / 2;
    }

    int left(int root) {
        return root * 2;
    }

    int right(int root) {
        return root * 2 + 1;
    }

    void swim(int k) {
        while (k > 1 && less(parent(k), k))
        {
            exch(parent(k), k);
            k = parent(k);
        }
    }

    void sink(int k) {
        while (left(k) <= N) 
        {
            int older = left(k);
            if (right(k) <= N && less(older, right(k))) older = right(k);
            if (less(older, k)) break;
            exch(k, older);
            k = older; 
        }
    }

    void exch(int i, int j) {
        Key temp = pq[i];
        pq[i] = pq[j];
        pq[j] = temp;
    }

    bool less(int i, int j) {
        return pq[i] < pq[j];
    }

private:
    Key* pq;
    int N = 0;
};

