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

//排序模板 
//1. 快速排序 O(nlgn)
void quick_sort(int q[], int l, int r) {
    if (l >= r) return;
    int i = l-1, j = r+1, x = q[l + (r-l)/2];
    while (i < j) {
        do i++; while (q[i] < x);
        do j--; while (q[j] > x);
        if (i < j) swap(q[i], q[j]);
    }
    quick_sort(q, l, j);
    quick_sort(q, j+1, r);
}

//2. 归并排序 稳定 O(nlgn)
void merge_sort(int q[], int l, int r) {
    if (l >= j) return;
    int mid = l + (r-l)/2;
    merge_sort(q, l, mid);
    merge_sort(q, mid+1, r);
    int k=0, i=l, j=mid+1;
    vector<int> temp;
    while(i<=mid && j<= r) {
        if (q[i] > q[j]) temp.push_back(q[j++]);
        else temp.push_back(q[i++]);
    }
    while (i<=mid) temp.push_back(q[i++]);
    while (j<=r) temp.push_back(q[j++]);
    for (int i=l; i<=r; i++) q[i] = temp[k++];
}

//3. 冒泡排序 稳定 O(n^2)
void bubble_sort(int q[], int n) {
    for (int i=n-1; i>0; i--)
        for (int j=0; j<i; j++) {
            if (q[j] > q[j+1]) swap(q[j], q[j+1]);
        }
}

//4. 插入排序 稳定 O(n^2) 
void insert_sort(int q[], int n) {
    for (int i=1; i<n; i++) {
        int temp = q[i];
        for (int j=i; j>0 && temp < q[j-1]; j--) q[j] = q[j-1];
        q[j] = temp;
    }
}

//5. 选择排序 O(n^2)
void select_sort(int q[], int n) {
    for (int i=0; i<n-1; i++) 
        for (int j=i+1; j<n; j++) {
            if (q[j] < q[i]) swap(q[i], q[j]);
        }
}

//6. 堆排序 O(nlgn)
void down(int q[], int n, int size) {
    int t = n;
    if (2*n <= size && q[2*n] < q[t]) t = 2*n;
    if (2*n+1 <= size && q[2*n+1] < q[t]) t = 2*n+1;
    if (t != n) {
        swap(q[t], q[n]);
        down(q, t, size);
    }
}

void up(int q[], int n, int size) {
    if (n/2 <= size && q[n/2] > q[n]) {
        swap(q[n/2], q[n]);
        up(q, n/2, size);
    }
}

void heap_sort(int q[]) {
    int n = q.size();
    int size_ = n;
    int t[n+1];
    copy(begin(q)+1, end(q), q)
    for (int i=n/2; i>0; i--) {
        down(t, i, q.size());
    }
    while (n--) {
        t[1] = t[size_];
        size_--;
        down(t, 1, q.size());
    }
}