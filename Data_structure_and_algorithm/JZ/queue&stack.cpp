//JZ9 用两个栈实现队列
void push(int node) {
    stack1.push(node);
}

int pop() {
    while (stack2.empty()) {
        while (stack1.size()) {
            int val = stack1.top();
            stack2.push(val);
            stack1.pop();
        }
    }
    int res = stack2.top();
    stack2.pop();
    return res;
}

stack<int> stack1;
stack<int> stack2;

//JZ30 包含min函数的栈
stack<int> stk, stk_min;
void push(int value) {
    stk.push(value);
    if (stk_min.empty() || stk_min.top() > stk.top()) stk_min.push(value);
    
}
void pop() {
    if (stk.top() == stk_min.top()) stk_min.pop();
    stk.pop();
    
}
int top() {
    return stk.top();
}
int min() {
    return stk_min.top();
}
//JZ31 栈的压入、弹出序列
bool IsPopOrder(vector<int> pushV,vector<int> popV) {
    if (pushV.empty() || popV.empty() || pushV.size() != popV.size()) return false;
    stack<int> stk;
    for (int i=0, j=0; i<pushV.size(); ++i)
    {
        stk.push(pushV[i]);
        while (stk.size() && stk.top()==popV[j]) stk.pop(), ++j;
    }
    if (stk.empty()) return true;
    return false;
}
//JZ73 翻转单词序列
string ReverseSentence(string str) {
    reverse(str.begin(), str.end());
    for (int i=0; i<str.size(); ++i) {
        int j = i;
        while (str[j] != ' ' && j < str.size()) ++j;
        reverse(str.begin()+i, str.begin()+j);
        i = j;
    }
    return str;
}
//JZ59 滑动窗口的最大值
vector<int> maxInWindows(const vector<int>& num, unsigned int size)
{
    vector<int> res;
    deque<int> q;
    for (int i=0; i<num.size(); ++i) {
        if (q.size() && i - q.front() >= size) q.pop_front();
        while (q.size() && num[q.back()] < num[i]) q.pop_back();
        q.push_back(i);
        if (i >= size-1) res.push_back(num[q.front()]);
    }
    return res;
}