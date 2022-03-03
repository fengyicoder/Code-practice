//JZ42 连续数组的最大和
int FindGreatestSumOfSubArray(vector<int> array) {
    int n = array.size();
    int res;
    vector<int> f(n+1);
    for (int i=1; i<=n; ++i) {
        f[i] = max(f[i-1]+array[i-1], array[i-1]);
    }
    res = INT_MIN;
    for (int i=1; i<=n; ++i) res = max(res, f[i]);
    return res;
}
//JZ69 跳台阶
int jumpFloor(int number) {
    int n = number;
    vector<int> f(n+1);
    f[0] = 1, f[1] = 1;
    for (int i=2; i<=n; i++)
    {
        f[i] = f[i-1] + f[i-2];
    }
    return f[n];
}
//JZ10 斐波那契数列
int Fibonacci(int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i)
    {
        int temp = b;
        b = a + b;
        a = temp;
    }
    return b;
}
//JZ71 跳台阶扩展
int jumpFloorII(int n) {
    vector<int> f(n+1);
    f[0] = 1,f[1] = 1;
    for (int i = 2; i <= n; i++) f[i] = 2 * f[i-1];
    return f[n];
}
//JZ70 矩形覆盖
int rectCover(int n) {
    vector<int> f(n+1);
    f[0] = 0, f[1] = 1, f[2]=2;
    for (int i=3; i<=n; ++i) f[i] = f[i-1] + f[i-2];
    return f[n];
}