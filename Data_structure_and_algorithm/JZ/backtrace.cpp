//JZ13 机器人的运动范围
int n, m;
int get_num(pair<int, int> num)
{
    int sum = 0;
    while (num.first)
    {
        sum += num.first % 10;
        num.first = num.first / 10;
    }
    while (num.second)
    {
        sum += num.second % 10;
        num.second = num.second / 10;
    }
    return sum;
    
}
int movingCount(int threshold, int rows, int cols)
{
    n = rows, m = cols;
    int res=0;
    if (!rows && !cols) return 0;
    queue<pair<int, int>> q;
    vector<vector<bool>> stat(n, vector<bool>(m, false));
    q.push({0,0});
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
    while (q.size())
    {
        pair<int, int> t = q.front();
        q.pop();
        if (stat[t.first][t.second] || get_num(t) > threshold) continue;
        stat[t.first][t.second] = true;
        res++;
        for (int i = 0; i < 4; i++)
        {
            int a = t.first + dx[i], b = t.second + dy[i];
            if (a >= 0 && a < n && b >= 0 && b < m) q.push({a, b});
        }
    }
    return res;
}