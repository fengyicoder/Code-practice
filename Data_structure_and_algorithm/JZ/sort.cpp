//JZ51 数组中的逆序对
static const auto io_sync_off = []() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    return nullptr;
}( );
class Solution {
public:
    static constexpr int P = 1000000007;
    vector<int>::iterator it;
    int InversePairs(vector<int>& data) {
        it = data.begin();
        if (data.empty())return 0;
        vector<int> dup(data);
        return merge_sort(data.begin(), data.end(), dup.begin());
    }
    //template<class RanIt>
    using RanIt = vector<int>::iterator;
    int merge_sort(const RanIt& begin1, const RanIt& end1, const RanIt& begin2) {
        int len = end1 - begin1;
        if (len < 2)return 0;
        int mid = ( len + 1 ) >> 1;
        auto m1 = begin1 + mid, m2 = begin2 + mid;
        auto i = m1, j = end1, k = begin2 + len;
        int ans = ( merge_sort(begin2, m2, begin1) + merge_sort(m2, k, m1) ) % P;
        for (--i, --j, --k; i >= begin1 && j >= m1; --k) {
            if (*i > *j) {
                *k = *i, --i;
                ( ans += j - m1 + 1 ) %= P;
            } else *k = *j, --j;
        }
        if (i >= begin1)copy(begin1, i + 1, begin2);
        else copy(m1, j + 1, begin2);
        return ans;
    }
  
};
//JZ40 最小的K个数
vector<int> GetLeastNumbers_Solution(vector<int> input, int k) {
    vector<int> res;
    if (input.empty() || input.size() < k) return res;
    priority_queue<int> stack;
    for (auto n : input) {
        stack.push(n);
        if (stack.size() > k) stack.pop();
    }
    while (stack.size()) {
        res.push_back(stack.top());
        stack.pop();
    }
    reverse(res.begin(), res.end());
    return res;
}
//JZ41 数据流中的中位数
priority_queue<int> max_heap;
priority_queue<int, vector<int>, greater<int>> min_heap;
void Insert(int num)
{
    max_heap.push(num);
    while (max_heap.size() > min_heap.size() + 1) {
        int t = max_heap.top();
        min_heap.push(t);
        max_heap.pop();
    }
    while (min_heap.size() && min_heap.top() < max_heap.top()) {
        auto k1 = max_heap.top(), k2 = min_heap.top();
        max_heap.pop();
        min_heap.pop();
        max_heap.push(k2);
        min_heap.push(k1);
    }
}

double GetMedian()
{ 
    if (max_heap.size() == min_heap.size()) return (max_heap.top() + min_heap.top()) / 2.0;
    return max_heap.top();
}