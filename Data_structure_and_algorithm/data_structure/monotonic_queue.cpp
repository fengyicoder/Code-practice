#include <deque>

using namespace std;

class MonotonicQueue {
private:
    deque<int> q;
public:
    void push(int n) {
        while(!q.empty() && q.front() < n) {
            q.pop_front();
        }
        q.push_back(n);
    }

    int max() {
        return q.front();
    }

    void pop(int n) {
        if (n == q.front()) q.pop_front();
    }
};