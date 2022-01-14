#include <queue>
#include <stack>

using namespace std;

class Queue {
private:
    stack<int> s1, s2;
public:
    void push(int n) {
        s1.push(n);
    }

    int peek() {
        if (s2.empty()) {
            while (!s1.empty()) {
                int n = s1.top();
                s2.push(n);
                s1.pop();
            }
        }
        return s2.top();
    }

    int pop() {
        peek();
        int n = s2.top();
        s2.pop();
        return n;
    }

    bool empty() {
        return s1.empty() && s2.empty();
    }
};

class Stack {
private:
    queue<int> q;
    int top_element;
public:
    void push(int n) {
        q.push(n);
        top_element = n;
    }

    int top() {
        return top_element;
    }

    int pop() {
        int size = q.size();
        while (size > 2)
        {
            int temp = q.front();
            q.pop();
            q.push(temp);
        }
        top_element = q.front();
        q.push(top_element);
        q.pop();
        int temp = q.front();
        q.pop();
        return temp;
    }

    bool empty() {
        return q.empty();
    }
};