//Leetcode 303 区域和检索
vector<int> presum;
NumArray(vector<int>& nums) {
    presum.resize(nums.size() + 1);
    for (int i=1; i<presum.size(); i++) {
        presum[i] = presum[i-1] + nums[i-1];
    }
}

int sumRange(int left, int right) {
    return presum[right+1] - presum[left];
}

//Leetcode 304 二维区域和检索
vector<vector<int>> sum;
NumMatrix(vector<vector<int>>& matrix) {
    int m = matrix.size();
    if (m > 0) {
        int n = matrix[0].size();
        sum.resize(m+1, vector<int>(n+1));
        for (int i=1; i<m+1; i++)
            for (int j=1; j<n+1; j++)
            {
                sum[i][j] = sum[i-1][j] + sum[i][j-1] + matrix[i-1][j-1] - sum[i-1][j-1];
            }
    }
}

int sumRegion(int row1, int col1, int row2, int col2) {
    return sum[row2+1][col2+1] - sum[row2+1][col1] - sum[row1][col2+1] + sum[row1][col1];
}

//Leetcode 560 和为k的子数组
int subarraySum(vector<int>& nums, int k) {
    int n = nums.size();
    std::unordered_map<int, int> hash;
    hash[0] = 1;
    int res = 0, presum = 0;
    for (int i=0; i<n; i++) {
        presum += nums[i];
        if (hash.count(presum - k)) {
            res += hash[presum-k];
        }
        hash[presum] = hash[presum]+1;
    }
    return res;
}

//Leetcode 370 区间加法
// 差分数组工具类
class Difference {
    // 差分数组
    private int[] diff;
    
    /* 输入一个初始数组，区间操作将在这个数组上进行 */
    public Difference(int[] nums) {
        assert nums.length > 0;
        diff = new int[nums.length];
        // 根据初始数组构造差分数组
        diff[0] = nums[0];
        for (int i = 1; i < nums.length; i++) {
            diff[i] = nums[i] - nums[i - 1];
        }
    }

    /* 给闭区间 [i,j] 增加 val（可以是负数）*/
    public void increment(int i, int j, int val) {
        diff[i] += val;
        if (j + 1 < diff.length) {
            diff[j + 1] -= val;
        }
    }

    /* 返回结果数组 */
    public int[] result() {
        int[] res = new int[diff.length];
        // 根据差分数组构造结果数组
        res[0] = diff[0];
        for (int i = 1; i < diff.length; i++) {
            res[i] = res[i - 1] + diff[i];
        }
        return res;
    }
}

public void increment(int i, int j, int val) {
    diff[i] += val;
    if (j + 1 < diff.length) {
        diff[j + 1] -= val;
    }
}

//Leetcode 1109 航班预订
class Difference {
private:
    vector<int> diff;
public:
    Difference(vector<int> &num) {
        int n = num.size();
        diff.resize(n);
        diff[0] = num[0];
        for (int i=1; i<n; ++i) {
            diff[i] = num[i] - num[i-1];
        }
    }

    void increment(int i, int j, int val) {
        diff[i] += val;
        if (j + 1 < diff.size()) {
            diff[j+1] -= val;
        }
    }

    vector<int> result() {
        vector<int> res(diff.size());
        res[0] = diff[0];
        for (int i=1; i<diff.size(); ++i) {
            res[i] = res[i-1] + diff[i];
        }
        return res;
    }
};
vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
    vector<int> num(n);
    Difference df(num);
    for (auto element: bookings) {
        df.increment(element[0]-1, element[1]-1, element[2]);
    }
    return df.result();
}

//Leetcode 1094 拼车
Difference(vector<int> &num) {
    int n = num.size();
    diff.resize(n);
    diff[0] = num[0];
    for (int i=1; i<n; ++i) {
        diff[i] = num[i] - num[i-1];
    }
}

void increment(int i, int j, int val) {
    diff[i] += val;
    if (j + 1 < diff.size()) {
        diff[j+1] -= val;
    }
}

vector<int> result() {
    vector<int> res(diff.size());
    res[0] = diff[0];
    for (int i=1; i<diff.size(); ++i) {
        res[i] = res[i-1] + diff[i];
    }
    return res;
}
};
bool carPooling(vector<vector<int>>& trips, int capacity) {
    vector<int> num(1001);
    Difference diff(num);
    for (auto e : trips) {
        int val = e[0];
        int left = e[1];
        int right = e[2]-1;
        diff.increment(left, right, val);
    }
    vector<int> res = diff.result();
    for (auto e : res) {
        if (e > capacity) return false;
    }
    return true;
}

//Leetcode 48 旋转图像
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int i=0; i<n; ++i)
        for (int j=i; j<n; ++j) {
            int tmp = matrix[i][j];
            matrix[i][j] = matrix[j][i];
            matrix[j][i] = tmp;
        }
    for (int i=0; i<n; ++i) reverse(matrix[i]);
}

void reverse(vector<int> &matrix) {
    int i = 0, j = matrix.size() - 1;
    while (i < j) {
        int temp = matrix[i];
        matrix[i] = matrix[j];
        matrix[j] = temp;
        i++;
        j--;
    }
}

//Leetcode 54 螺旋矩阵
vector<int> spiralOrder(vector<vector<int>>& matrix) {
    int n = matrix.size(), m = matrix[0].size();
    int up = 0, down = n - 1, left = 0, right = m - 1;
    vector<int> res;
    while(res.size() < m*n) {
        if (up <= down) {
            for (int i=left; i<=right; ++i) res.push_back(matrix[up][i]);
            ++up;
        }
        if (left <= right) {
            for (int i=up; i<=down; ++i) res.push_back(matrix[i][right]);
            --right;
        }
        if (up <= down) {
            for (int i=right; i>=left; --i) res.push_back(matrix[down][i]);
            --down;
        }
        if (left <= right) {
            for (int i=down; i>=up; --i) res.push_back(matrix[i][left]);
            ++left;
        }
    }
    return res;
}

//Leetcode 59 螺旋矩阵2
vector<vector<int>> generateMatrix(int n) {
    int up = 0, down = n - 1, left = 0, right = n - 1;
    vector<vector<int>> res(n, vector<int>(n));
    int num = 0;
    while(num < n*n) {
        if (up <= down) {
            for (int i=left; i<=right; ++i) res[up][i] = ++num;
            ++up;
        }
        if (left <= right) {
            for (int i=up; i<=down; ++i) res[i][right] = ++num;
            --right;
        }
        if (up <= down) {
            for (int i=right; i>=left; --i) res[down][i] = ++num;
            --down;
        }
        if (left <= right) {
            for (int i=down; i>=up; --i) res[i][left] = ++num;
            ++left;
        }
    }
    return res;
}

//二分搜索
int binary_search(int[] nums, int target) {
    int left = 0, right = nums.length - 1; 
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1; 
        } else if(nums[mid] == target) {
            // 直接返回
            return mid;
        }
    }
    // 直接返回
    return -1;
}

int left_bound(int[] nums, int target) {
    int left = 0, right = nums.length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定左侧边界
            right = mid - 1;
        }
    }
    // 最后要检查 left 越界的情况
    if (left >= nums.length || nums[left] != target) {
        return -1;
    }
    return left;
}

int right_bound(int[] nums, int target) {
    int left = 0, right = nums.length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定右侧边界
            left = mid + 1;
        }
    }
    // 最后要检查 right 越界的情况
    if (right < 0 || nums[right] != target) {
        return -1;
    }
    return right;
}

//Leetcode 167 两数之和2
vector<int> twoSum(vector<int>& numbers, int target) {
    int left = 0, right = numbers.size()-1;
    while (left < right) {
        int sum = numbers[left] + numbers[right];
        if (sum == target) return vector<int>{left+1, right+1};
        else if (sum < target) left++;
        else if (sum > target) right--;
    }
    return vector<int>{-1, -1};
}

//Leetcode 344 翻转字符串
void reverseString(vector<char>& s) {
    int left = 0, right = s.size()-1;
    while (left < right) {
        char tmp = s[right];
        s[right] = s[left];
        s[left] = tmp;
        left++, right--;
    }
}

//Leetcode 76 最小覆盖子串
string minWindow(string s, string t) {
    unordered_map<char, int> need, window;
    int valid = 0, start = 0, len = INT_MAX;
    int left = 0, right = 0;
    for (auto c : t) need[c]++;
    while(right < s.size()) {
        auto c = s[right];
        ++right;
        if (need.count(c)) {
            window[c]++;
            if (window[c] == need[c]) valid++;
        }
        while (valid == need.size()) {
            if (right - left < len) {
                start = left;
                len = right - left;
            }
            auto c = s[left];
            ++left;
            if (need.count(c)) {
                if (need[c] == window[c]) {
                    valid--;
                }
                window[c]--;
            }
        }
    }
    return len == INT_MAX ? "" : s.substr(start, len);
}

//Leetcode 567 字符串的排列
bool checkInclusion(string s1, string s2) {
    unordered_map<char, int> need, window;
    for (auto c : s1) need[c]++;
    int valid = 0, left = 0, right = 0;
    while (right < s2.size()) {
        char c = s2[right];
        right++;
        if (need.count(c)) {
            window[c]++;
            if (need[c] == window[c]) valid++;
        }

        while(right - left >= s1.size()) {
            if (valid == need.size()) return true;
            char c = s2[left];
            left++;
            if (need.count(c)) {
                if (need[c] == window[c]) valid--;
                window[c]--;
            }
        }
    }
    return false;
}

//Leetcode 438 找到字符串中所有字母异位词
vector<int> findAnagrams(string s, string p) {
    unordered_map<char, int> need, window;
    for (auto c : p) need[c]++;
    int valid = 0, left = 0, right = 0;
    vector<int> res;
    while (right < s.size()) {
        char c = s[right];
        right++;
        if (need.count(c)) {
            window[c]++;
            if (need[c] == window[c]) valid++;
        }

        while(right - left >= p.size()) {
            if (valid == need.size()) res.push_back(left);
            char c = s[left];
            left++;
            if (need.count(c)) {
                if (need[c] == window[c]) valid--;
                window[c]--;
            }
        }
    }
    return res;
}

//Leetcode 3 最长无重复子串
int lengthOfLongestSubstring(string s) {
    unordered_map<char, int> window;
    int left = 0, right = 0;
    int res = 0;
    while (right < s.size()) {
        char c = s[right];
        right++;
        window[c]++;
        while(window[c] > 1) {
            char c = s[left];
            left++;
            window[c]--;
        }
        res = max(res, right-left);
    }
    return res;
}