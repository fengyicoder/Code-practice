#include <queue>

using namespace std;

class MediaFinder {
private:
    priority_queue<int, vector<int>, greater<int> > large;
    priority_queue<int, vector<int>, less<int> > small;

    double findMedian() {
        if (large.size() > small.size()) return large.top();
        else if (large.size() < small.size()) return small.top();
        return (large.top() + small.top()) / 2.0;
    }
    
    void addNum(int num) {
        if (small.size() >= large.size()) {
            small.push(num);
            large.push(small.top());
            small.pop();
        }
        else {
            large.push(num);
            small.push(large.top());
            large.pop();
        }
    }
};