//JZ65 不用加减乘除做加法
int Add(int num1, int num2)
{
    while (num2)
    {
        int sum = num1 ^ num2;
        int carry = (num1 & num2) << 1;
        num1 = sum;
        num2 = carry;
    }
    return num1;
}
//JZ15 二进制中1的个数
int  NumberOf1(int n) {
    unsigned int n_ = n;
    int res = 0;
    while (n_) res += n_ & 1, n_ >>= 1;
    return res;
}
//JZ16 数值的整数次方
def Power(self, base, exponent):
    return base**exponent
//JZ64 求1+2+..+n
int Sum_Solution(int n) {
    int sum = n;
    n > 0 && (sum += Sum_Solution(n-1));
    return sum;
}
//JZ29 顺时针打印矩阵
vector<int> printMatrix(vector<vector<int> > matrix) {
    int n = matrix.size(), m = matrix[0].size();
    vector<int> res;
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
    int x = 0, y = 0, d = 1;
    vector<vector<bool>> state(n, vector<bool>(m, false));
    for (int i=0; i<n*m; ++i)
    {
        res.push_back(matrix[x][y]);
        state[x][y] = true;
        int a = x + dx[d], b = y + dy[d];
        if (a < 0 || a >= n || b < 0 || b >= m || state[a][b])
        {
            d = (d+1) % 4;
            a = x + dx[d];
            b = y + dy[d];
        }
        x = a, y = b;
    }
    return res;
}
//JZ61 扑克牌顺子
bool IsContinuous( vector<int> numbers ) {
    if (numbers.empty()) return false;
    sort(numbers.begin(), numbers.end());
    int i = 0;
    while (numbers[i] == 0) ++i;
    for (int k=i+1; k<numbers.size(); ++k)
        if (numbers[k] == numbers[k-1]) return false;
    return numbers.back() - numbers[i] <= 4;
}
//JZ66 构建乘积数组
vector<int> multiply(const vector<int>& A) {
    int n = A.size();
    vector<int> B(n);
    for (int i=0, p=1; i<n; ++i) {
        B[i] = p;
        p *= A[i];
    }
    for (int j=n-1, p=1; j>=0; --j) {
        B[j] *= p;
        p *= A[j];
    }
    return B;
}
//JZ50 第一个只出现一次的字符
int FirstNotRepeatingChar(string str) {
    unordered_map<char, int> hash;
    for (auto s : str) hash[s]++;
    for (int i=0; i<str.size(); ++i)
    {
        if (hash[str[i]] == 1) return i;
    }
    return -1;
}
//JZ39 数组中出现次数超过一半的数字
def MoreThanHalfNum_Solution(self, numbers):
    hash = {};
    n = len(numbers)
    for i in numbers:
        hash[i] = hash[i] + 1 if hash.get(i) else 1
        if hash[i] > int(n/2):
            return i 
    return 0
//JZ43 整数中1出现的次数
def NumberOf1Between1AndN_Solution(self, n):
    # write code here
    cnt = 0
    for i in range(1, n+1):
        while (i):
            t = i % 10
            i = i / 10
            if t == 1: cnt += 1
    return cnt
//JZ45 把数组排成最小的数
 static bool cmp(int &a, int &b)
{
    if (to_string(a)+to_string(b) < to_string(b)+to_string(a)) return true;
    else return false;
}
string PrintMinNumber(vector<int> numbers) {
    string res;
    sort(numbers.begin(), numbers.end(), cmp);
    for (int i=0; i<numbers.size(); ++i) {
        res += to_string(numbers[i]);
    }
    return res;
}
//JZ49 丑数
int GetUglyNumber_Solution(int index) {
    if (index <= 1) return index;
    vector<int> f(1,1);
    int i = 0, j = 0, k = 0;
    while (--index) {
        int t = min(2*f[i], min(3*f[j], 5*f[k]));
        if (t == 2*f[i]) ++i;
        if (t == 3*f[j]) ++j;
        if (t == 5*f[k]) ++k;
        f.push_back(t);
    }
    return f.back();
}
//JZ74 和为S的连续正数序列
vector<vector<int> > FindContinuousSequence(int sum) {
    vector<vector<int>> res;
    for (int i=1, j=1, s=1; i<=sum; ++i)
    {
        while (s < sum && j <= sum) s += ++j;
        if (s == sum && j-i+1 > 1)
        {
            vector<int> line;
            for (int k=i; k<=j; ++k) line.push_back(k);
            res.push_back(line);
            
        }
        s -= i;
    }
    return res;
}
//JZ57 和为S的两个数字
def FindNumbersWithSum(self, array, tsum):
    for i in array:
        if tsum - i in array:
            return [i, tsum-i]
    return []

// JZ58 左旋转字符串
string LeftRotateString(string str, int n) {
        reverse(str.begin(), str.end());
        reverse(str.begin(), str.begin()+str.size()-n);
        reverse(str.begin()+str.size()-n, str.end());
        return str;
    }
//JZ62 孩子们的游戏（圆圈中最后剩下的数）
int LastRemaining_Solution(int n, int m)
{
    if (n == 0) return -1;
    if (n == 1) return 0;
    return (LastRemaining_Solution(n-1, m) + m) % n;
}
//JZ75 字符流中第一个不重复的字符
queue<int> q;
unordered_map<char, int> hash;
void Insert(char ch)
{
        if (++hash[ch] > 1)
            while (q.size() && hash[q.front()] > 1) q.pop();
        else q.push(ch);
}
//return the first appearence once char in current stringstream
char FirstAppearingOnce()
{
    if (q.empty()) return '#';
    return q.front();
}
//JZ14 剪绳子
 int cutRope(int number) {
    if (number <= 3) return number-1;
    int res = 1;
    if (number % 3 == 1) res *= 4, number -= 4;
    if (number % 3 == 2) res *= 2, number -= 2;
    while (number) {
        res *= 3;
        number -= 3;
    }
    return res;
}