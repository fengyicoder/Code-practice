//Leetcode 20 有效的括号
bool isValid(string s) {
    stack<char> left;
    for (auto c : s) {
        if (c == '(' || c == '[' || c == '{') left.push(c);
        else {
            if (!left.empty() && leftOf(c) == left.top()) left.pop();
            else return false;
        }
    }
    return left.empty();
}
char leftOf(char c) {
    if (c == '}') return '{';
    else if (c == ')') return '(';
    else return '[';
}

//Leetcode 921 使括号有效的最少添加
int minAddToMakeValid(string s) {
    int res = 0, need = 0;
    for (auto c : s) {
        if (c == '(') need++;
        else if (c == ')') {
            need--;
            if (need == -1) need = 0, res++;
        }
    }
    return res + need;
}

//Leetcode 1541 平衡括号字符串的最少插入次数
int minInsertions(string s) {
    int need = 0, res = 0;
    for (auto c : s) {
        if (c == '(') {
            need += 2;
            if (need % 2 == 1) {
                res++;
                need--;
            }
        }
        else if (c == ')') {
            need--;
            if (need == -1) {
                need = 1;
                res++;
            }
        }
    }
    return res + need;
}

//Leetcode 22 括号生成
vector<string> generateParenthesis(int n) {
    if (n == 0) return  {};
    vector<string> res;
    string track;
    backtrace(n, n, track, res);
    return res;
}

void backtrace(int left, int right, string &track, vector<string> &res) {
    if (right < left) return;
    if (left < 0 || right < 0) return;
    if (right == 0 && left == 0) {
        res.push_back(track);
        return;
    }
    track.push_back('(');
    backtrace(left-1, right, track, res);
    track.pop_back();

    track.push_back(')');
    backtrace(left, right-1, track, res);
    track.pop_back();
}

//BFS算法框架
// 计算从起点 start 到终点 target 的最近距离

int BFS(Node start, Node target) {
    Queue<Node> q; // 核心数据结构
    Set<Node> visited; // 避免走回头路

    q.offer(start); // 将起点加入队列
    visited.add(start);
    int step = 0; // 记录扩散的步数

    while (q not empty) {
        int sz = q.size();
        /* 将当前队列中的所有节点向四周扩散 */
        for (int i = 0; i < sz; i++) {
            Node cur = q.poll();
            /* 划重点：这里判断是否到达终点 */
            if (cur is target)
                return step;
            /* 将 cur 的相邻节点加入队列 */
            for (Node x : cur.adj()) {
                if (x not in visited) {
                    q.offer(x);
                    visited.add(x);
                }
            }
        }
        /* 划重点：更新步数在这里 */
        step++;
    }
}

//Leetcode 111 二叉树的最小深度
int minDepth(TreeNode* root) {
    if (!root) return 0;
    queue<TreeNode*> q;
    int depth = 1;
    q.push(root);
    while (q.size()) {
        int size = q.size();
        for (int i=0; i<size; i++) {
            auto node = q.front();
            q.pop();
            if (!node->left && !node->right) {
                return depth;
            }
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        depth++;
    }
    return depth;
}

//Leetcode 752 打开转盘锁
int openLock(vector<string>& deadends, string target) {
    queue<string> q;
    set<string> visit;
    set<string> dead;
    int step = 0;
    for (auto s : deadends) dead.insert(s);
    string num = "0000";
    q.push(num);
    visit.insert(num);
    while (!q.empty()) {
        int sz = q.size();
        for (int i=0; i<sz; i++) {
            string cur = q.front();
            q.pop();
            if (dead.count(cur)) continue;
            if (cur == target) return step;
            for (int j=0; j<4; j++) {
                string up = plusOne(cur, j);
                if (!visit.count(up)) {
                    q.push(up);
                    visit.insert(up);
                }
                string down = minusOne(cur, j);
                if (!visit.count(down)) {
                    q.push(down);
                    visit.insert(down);
                }
            }
        }
        step++;
    }
    return -1;
}

string plusOne(string s, int i) {
    if (s[i] == '9') s[i] = '0';
    else s[i] = s[i] + 1;
    return s;
}

string minusOne(string s, int i) {
    if (s[i] == '0') s[i] = '9';
    else s[i] = s[i] - 1;
    return s;
}

//Leetcode 773 滑动谜题
int slidingPuzzle(vector<vector<int>>& board) {
    int n = 2, m = 3;
    string sb = "";
    string target = "123450";
    for (int i=0; i<n; i++)
        for (int j=0; j<m; j++) {
            sb += to_string(board[i][j]);
        }
    vector<vector<int>> state({{1, 3}, {0, 2, 4}, {1, 5}, {0, 4}, {1, 3, 5}, {2, 4}});
    queue<string> q;
    set<string> visited;
    int step = 0;
    q.push(sb);
    visited.insert(sb);
    while (q.size()) {
        int sz = q.size();
        for (int i=0; i<sz; i++) {
            string cur = q.front();
            q.pop();
            if (cur == target) return step;
            int index = 0;
            while (cur[index] != '0') {
                index++;
            }
            for (auto i : state[index]) {
                string change = cus_swap(cur, i, index);
                if (!visited.count(change)) {
                    q.push(change);
                    visited.insert(change);
                }
            }
        }
        step++;
    }
    return -1;
}

string cus_swap(string s, int i, int j) {
    char tmp = s[j];
    s[j] = s[i];
    s[i] = tmp;
    return s;
}