//Leetcode 46 全排列
vector<vector<int>> res;
vector<vector<int>> permute(vector<int>& nums) {
    vector<int> track;
    backtrace(nums, track);
    return res;
}

void backtrace(vector<int>& nums, vector<int>& track) {
    if (track.size() == nums.size()) {
        res.push_back(track);
        return;
    }
    for (auto n : nums) {
        auto iter = find(track.begin(), track.end(), n);
        if (iter != track.end()) {
            continue;
        }
        track.push_back(n);
        backtrace(nums, track);
        track.pop_back();
    }
}

//Leetcode 51 N皇后
vector<vector<string>> res;
vector<vector<string>> solveNQueens(int n) {
    vector<string> board(n, string(n, '.'));
    backtrace(board, 0);
    return res;
}

void backtrace(vector<string>& board, int row) {
    if (row == board.size()) {
        res.push_back(board);
        return;
    }
    int n = board.size();
    for (int col=0; col<n; col++) {
        if (!isValid(board, row, col)) continue;
        board[row][col] = 'Q';
        backtrace(board, row+1);
        board[row][col] = '.';
    }
}

bool isValid(vector<string>& board, int row, int col) {
    int n = board.size();
    for (int i=0; i<n; i++) {
        if (board[i][col] == 'Q') return false;
    }

    for (int i=row-1, j=col+1; i>=0 && j<n; i--, j++) {
        if (board[i][j] == 'Q') return false;
    }

    for (int i=row-1, j=col-1; i>=0 && j>=0; i--, j--) {
        if (board[i][j] == 'Q') return false;
    }
    return true;
}

//Leetcode 698 划分为k个相等的子集
unordered_map<int, bool> hash;
bool canPartitionKSubsets(vector<int>& nums, int k) {
    if (k > nums.size()) return false;
    int sum = 0;
    for (auto n : nums) sum += n;
    if (sum % k) return false;
    int target = sum / k;
    int used = 0;
    return backtrace(k, 0, nums, 0, used, target);
}

bool backtrace(int k, int bucket, vector<int>& nums, int start, int used, int target) {
    if (k == 0) {
        return true;
    }
    if (bucket == target) {
        bool res = backtrace(k-1, 0, nums, 0, used, target);
        hash.insert({used, res});
        return res;
    }
    if (hash.count(used)) return hash[used];
    for (int i=start; i<nums.size(); i++) {
        if ((used >> i) & 1) continue;
        if (bucket + nums[i] > target) continue;
        used |= 1 << i;
        bucket += nums[i];
        if (backtrace(k, bucket, nums, i+1, used, target)) return true;
        used ^= 1 << i;
        bucket -= nums[i];
    }
    return false;
}

//Leetcode 78 子集
vector<vector<int>> res;
vector<vector<int>> subsets(vector<int>& nums) {
    vector<int> track;
    backtrace(nums, 0, track);
    return res;
}

void backtrace(vector<int>& nums, int start, vector<int>track) {
    res.push_back(track);
    for (int i=start; i<nums.size(); i++) {
        track.push_back(nums[i]);
        backtrace(nums, i+1, track);
        track.pop_back();
    }
}

//Leetcode 77 组合
vector<vector<int>> res;
vector<vector<int>> combine(int n, int k) {
    if (n <= 0 || k <= 0) return res;
    vector<int> track;
    backtrace(n, k, 1, track);
    return res;
}

void backtrace(int n, int k, int start, vector<int>track) {
    if (track.size() == k) {
        res.push_back(track);
        return;
    }
    for (int i=start; i<=n; i++) {
        track.push_back(i);
        backtrace(n, k, i+1, track);
        track.pop_back();
    }
}

//Leetcode 37 解数独
void solveSudoku(vector<vector<char>>& board) {
    backtrace(board, 0, 0);
}

bool backtrace(vector<vector<char>>& board, int i, int j) {
    int n = 9, m = 9;
    if (j == m) return backtrace(board, i+1, 0);
    if (i == n) return true;
    if (board[i][j] != '.') return backtrace(board, i, j+1);
    for (char ch='1'; ch<='9'; ch++) {
        if (!isValid(board, i, j, ch)) continue;
        board[i][j] = ch;
        if (backtrace(board, i, j+1)) return true;
        board[i][j] ='.';
    }
    return false;
}

bool isValid(vector<vector<char>>& board, int r, int c, char target) {
    for (int i=0; i<9; i++) {
        if (board[r][i] == target) return false;
        if (board[i][c] == target) return false;
        if (board[(r/3)*3+i/3][(c/3)*3+i%3] == target) return false;
    }
    return true;
}