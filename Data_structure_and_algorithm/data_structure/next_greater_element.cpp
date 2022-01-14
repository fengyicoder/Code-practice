#include <stack>
#include <vector>

using namespace std;

vector<int> nextGreaterElement(vector<int>& nums) {
    vector<int> res(nums.size());
    stack<int> s;
    for (int i=nums.size()-1; i>=0; i--) {
        while (!s.empty() && s.top() < nums[i]) s.pop();
        res[i] = s.empty() ? -1 : s.top();
        s.push(nums[i]);
    }
    return res;
}

vector<int> dailyTemperatures(vector<int>& T) {
    vector<int> res(T.size());
    // 这里放元素索引，而不是元素
    stack<int> s; 
    /* 单调栈模板 */
    for (int i = T.size() - 1; i >= 0; i--) {
        while (!s.empty() && T[s.top()] <= T[i]) {
            s.pop();
        }
        // 得到索引间距
        res[i] = s.empty() ? 0 : (s.top() - i); 
        // 将索引入栈，而不是元素
        s.push(i); 
    }
    return res;
}

//处理环形数组的情况
vector<int> nextGreaterElement(vector<int>& nums) {
    vector<int> res(nums.size());
    stack<int> s;
    int n = nums.size();
    for (int i=2 * n - 1; i>=0; i--) {
        while (!s.empty() && s.top() < nums[i]) s.pop();
        res[i % n] = s.empty() ? -1 : s.top();
        s.push(nums[i % n]);
    }
    return res;
}