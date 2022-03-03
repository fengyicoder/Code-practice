//Leetcode 509 斐波那契数列
int fib(int n) {
    if (n==0 || n==1) return n;
    int dp1 = 1, dp2 = 0;
    for (int i=2; i<=n; ++i) {
        int dp = dp1 + dp2;
        dp2 = dp1;
        dp1 = dp;
    }
    return dp1;
}

//Leetcode 322 零钱兑换
//顶层写法
vector<int> memo;
int coinChange(vector<int>& coins, int amount) {
    memo = vector<int>(amount + 1, -666);
    return dp(coins, amount);
}

int dp(vector<int>& coins, int amount) {
    if (amount == 0) return 0;
    if (amount < 0) return -1;
    if(memo[amount] != -666) return memo[amount];
    int res = INT_MAX;
    for (auto c : coins) {
        int sub = dp(coins, amount - c);
        if (sub == -1) continue;
        res = min(res, sub+1);
    }
    memo[amount] = res == INT_MAX ? -1 : res;
    return memo[amount];
}
//底层写法
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount+1, amount+1);
    dp[0] = 0;
    for (int i=0; i<dp.size(); i++) 
        for (auto c : coins) {
            if (i - c < 0) continue;
            dp[i] = min(dp[i], dp[i-c] + 1);
        }
    return dp[amount] == amount+1 ? -1: dp[amount];
}

//Leetcode 300 最长递增子序列
int lengthOfLIS(vector<int>& nums) {
    vector<int> dp(nums.size(), 1);
    int res = 0;
    for (int i=0; i<nums.size(); i++)
        for (int j=0; j<i; j++) {
            if (nums[j] < nums[i]) dp[i] = max(dp[i], dp[j] + 1);
        }
    for (auto m : dp) {
        if (m > res) res = m;
    }
    return res;
}

//下降路径最小和
vector<vector<int>> memo;
int minFallingPathSum(vector<vector<int>>& matrix) {
    int n = matrix.size();
    memo = vector<vector<int>>(n, vector<int>(n, 66666));
    int res = INT_MAX;
    for (int i=0; i<n; i++) {
        res = min(res, dp(matrix, n-1, i));
    }
    return res;
}

int dp(vector<vector<int>>& matrix, int i, int j) {
    if (i < 0 || j < 0 || i >= matrix.size() || j >= matrix[0].size()) {
        return 99999;
    }
    if (i == 0) return matrix[0][j];
    if (memo[i][j] != 66666) return memo[i][j];
    memo[i][j] = matrix[i][j] + cusmin(dp(matrix, i-1, j-1), dp(matrix, i-1, j), dp(matrix, i-1, j+1));
    return memo[i][j];
}

int cusmin(int a, int b, int c) {
    return min(a, min(b, c));
}

//Leetcode 494 目标和
//回溯法
int result = 0;
int findTargetSumWays(vector<int>& nums, int target) {
    if (nums.size() == 0) return 0;
    backtrace(nums, 0, target);
    return result;
}

void backtrace(vector<int>& nums, int i, int res) {
    if (i == nums.size()) {
        if (res == 0) ++result;
        return;
    }

    res += nums[i];
    backtrace(nums, i+1, res);
    res -= nums[i];

    res -= nums[i];
    backtrace(nums, i+1, res);
    res += nums[i];
}

//采用备忘录的解法
unordered_map<string, int> hash;
int findTargetSumWays(vector<int>& nums, int target) {
    if (nums.size() == 0) return 0;
    return backtrace(nums, 0, target);
}

int backtrace(vector<int>& nums, int i, int res) {
    if (i == nums.size()) {
        if (res == 0) return 1;
        return 0;
    }

    string key = to_string(i) + "," + to_string(res);
    if (hash.count(key)) return hash[key];

    int result = backtrace(nums, i+1, res-nums[i]) + backtrace(nums, i+1, res+nums[i]);
    hash[key] = result;
    return result;
}

//Leetcode 1143 最长公共子序列
//顶层方法
vector<vector<int>> memo;
int longestCommonSubsequence(string text1, string text2) {
    int n = text1.size(), m = text2.size();
    memo = vector<vector<int>>(n, vector<int>(m, -1));
    return dp(text1, text2, 0, 0);
}

int dp(string& text1, string& text2, int i, int j) {
    if (text1.size() == i || text2.size() == j) return 0;
    if (memo[i][j] != -1) return memo[i][j];
    if (text1[i] == text2[j]) {
        memo[i][j] = 1 + dp(text1, text2, i+1, j+1);
    }
    else {
        memo[i][j] = max(dp(text1, text2, i+1, j), dp(text1, text2, i, j+1));
    }
    return memo[i][j];
}
//底层方法
int longestCommonSubsequence(string text1, string text2) {
    int n = text1.size(), m = text2.size();
    vector<vector<int>> memo(n+1, vector<int>(m+1, 0));
    for (int i=1; i<=n; ++i)
        for (int j=1; j<=m; ++j) {
            if (text1[i-1] == text2[j-1]) memo[i][j] = 1 + memo[i-1][j-1];
            else memo[i][j] = max(memo[i-1][j], memo[i][j-1]);
        }
    return memo[n][m];
}

//Leetcode 583 两个字符串的删除操作
int minDistance(string word1, string word2) {
    int len = minLCS(word1, word2);
    return word1.size() + word2.size() - 2 * len;
}
int minLCS(string& word1, string& word2) {
    int n = word1.size(), m = word2.size();
    vector<vector<int>> memo(n+1, vector<int>(m+1, 0));
    for (int i=1; i<=n; ++i)
        for (int j=1; j<=m; ++j) {
            if (word1[i-1] == word2[j-1]) memo[i][j] = memo[i-1][j-1] + 1;
            else memo[i][j] = max(memo[i-1][j], memo[i][j-1]);
        }
    return memo[n][m];
}

//Leetcode 712 字符串的最小ascii和
int minimumDeleteSum(string s1, string s2) {
    int n = s1.size(), m = s2.size();
    vector<vector<int>> memo(n+1, vector<int>(m+1, 0));
    for (int i=1; i<=n; ++i) memo[i][0] = memo[i-1][0] + (int)s1[i-1];
    for (int j=1; j<=m; ++j) memo[0][j] = memo[0][j-1] + (int)s2[j-1];
    for (int i=1; i<=n; ++i)
        for (int j=1; j<=m; ++j) {
            if (s1[i-1] == s2[j-1]) memo[i][j] = memo[i-1][j-1];
            else memo[i][j] = min(memo[i-1][j] + (int)s1[i-1], memo[i][j-1] + (int)s2[j-1]);
        }
    return memo[n][m];
}

//Leetcode 72 编辑距离
int minDistance(string word1, string word2) {
    int n = word1.size(), m = word2.size();
    vector<vector<int>> memo(n+1, vector<int>(m+1, 0));
    for (int i=1; i<=n; ++i) memo[i][0] = i;
    for (int j=1; j<=m; ++j) memo[0][j] = j;

    for (int i=1; i<=n; ++i)
        for (int j=1; j<=m; ++j) {
            if (word1[i-1] == word2[j-1]) memo[i][j] = memo[i-1][j-1];
            else {
                memo[i][j] = cus_min(memo[i-1][j], memo[i][j-1], memo[i-1][j-1]) + 1;
            }
        }
    return memo[n][m];
}

int cus_min(int a, int b, int c) {
    return min(a, min(b, c));
}

//Leetcode 354 俄罗斯套娃信封问题
int maxEnvelopes(vector<vector<int>>& envelopes) {
    int n = envelopes.size();
    sort(envelopes.begin(), envelopes.end(), [](auto &e1, auto &e2){
        return e1[0] == e2[0] ? e2[1] < e1[1] : e1[0] < e2[0];
    });
    auto res = vector<int>(n, 0);
    for (int i=0; i<n; i++) {
        res[i] = envelopes[i][1];
    }
    return lis(res);
}

int lis(vector<int>& array) {
    if (array.empty()) return 0;
    int n = array.size();
    int res = 0;
    vector<int> memo(n, 1);
    for (int i=0; i<n; i++) 
        for (int j=0; j<i; j++) {
            if (array[j] < array[i]) memo[i] = max(memo[j]+1, memo[i]);
        }
    for (auto num : memo) {
        if (num > res) res = num;
    }
    return res;
}

//Leetcode 53 最大子数组和
int maxSubArray(vector<int>& nums) {
    if (nums.empty()) return 0;
    int n = nums.size();
    int res = INT_MIN;
    vector<int> memo(n, 0);
    memo[0] = nums[0];
    for (int i=1; i<n; i++) {
        memo[i] = max(memo[i-1]+nums[i], nums[i]);
    }
    for (auto num : memo) {
        if (num > res) res = num;
    }
    return res;
}

//Leetcode 516 最长回文子序列
int longestPalindromeSubseq(string s) {
    if (s.empty()) return 0;
    int n = s.size();
    vector<vector<int>> memo(n, vector<int>(n, 0));
    for (int i=0; i<n; i++) {
        memo[i][i] = 1;
    }
    for (int i=n-1; i>=0; i--)
        for (int j=i+1; j<n; j++) {
            if (s[i] == s[j]) memo[i][j] = memo[i+1][j-1] + 2;
            else memo[i][j] = max(memo[i+1][j], memo[i][j-1]);
        }
    return memo[0][n-1];
}

//0-1背包问题
int knapsack(int W, int N, vector<int>& wt, vector<int>& val) {
    // base case 已初始化
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));
    for (int i = 1; i <= N; i++) {
        for (int w = 1; w <= W; w++) {
            if (w - wt[i-1] < 0) {
                // 这种情况下只能选择不装入背包
                dp[i][w] = dp[i - 1][w];
            } else {
                // 装入或者不装入背包，择优
                dp[i][w] = max(dp[i - 1][w - wt[i-1]] + val[i-1], 
                               dp[i - 1][w]);
            }
        }
    }
    
    return dp[N][W];
}

//Leetcode 416 分割等和子集
bool canPartition(vector<int>& nums) {
    int sum = 0;
    for (auto n : nums) sum += n;
    if (sum % 2) return false;
    sum = sum / 2;
    int n = nums.size();
    vector<vector<bool>> memo(n+1, vector<bool>(sum+1, false));
    for (int i=0; i<=n; i++) memo[i][0] = true;
    for (int i=1; i<=n; i++)
        for (int j=1; j<=sum; j++) {
            if (j-nums[i-1] < 0) memo[i][j] = memo[i-1][j];
            else {
                memo[i][j] = memo[i-1][j] || memo[i-1][j-nums[i-1]];
            }
        }
    return memo[n][sum];
}

//Leetcode 528 零钱兑换2
int change(int amount, vector<int>& coins) {
    int n = coins.size();
    vector<vector<int>> memo(n+1, vector<int>(amount+1, 0));
    for (int i=0; i<=n; i++) memo[i][0] = 1;
    for (int i=1; i<=n; i++)
        for (int j=1; j<=amount; j++)
        {
            if (j - coins[i-1] < 0) memo[i][j] = memo[i-1][j];
            else memo[i][j] = memo[i-1][j] + memo[i][j-coins[i-1]];
        }
    return memo[n][amount];
}