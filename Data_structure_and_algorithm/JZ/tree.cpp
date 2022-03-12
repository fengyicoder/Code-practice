//二叉树的深度
int TreeDepth(TreeNode* pRoot)
{
    if (!pRoot) return 0;
    return max(TreeDepth(pRoot->left), TreeDepth(pRoot->right)) + 1; 
}

//JZ77 按之字形顺序打印二叉树
vector<vector<int> > Print(TreeNode* pRoot) {
    vector<vector<int>> res;
    vector<int> cur;
    queue<TreeNode*> q;
    if (!pRoot) return res;
    q.push(pRoot);
    bool flag = false;
    while (q.size()) {
        int n = q.size();
        for (int i=0; i<n; i++) {
            TreeNode* t = q.front();
            cur.push_back(t->val);
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
            q.pop();
        }
        flag = !flag;
        if (flag) res.push_back(cur);
        else reverse(cur.begin(), cur.end()), res.push_back(cur);
        cur.clear();
    }
    return res;
}

//JZ7 重建二叉树
unordered_map<int, int> map;
TreeNode* reConstructBinaryTree(vector<int> pre,vector<int> vin) {
    int n = pre.size();
    for (int i=0; i<n; i++) map[vin[i]] = i;
    return dfs(pre, vin, 0, n-1, 0, n-1);

}
TreeNode* dfs(vector<int> pre,vector<int> vin, int pl, int pr, int vl, int vr)
{
    if (pl > pr) return NULL;
    int val = pre[pl];
    int lngth = map[val] - vl;
    TreeNode* root = new TreeNode(val);
    root->left = dfs(pre, vin, pl+1, pl+lngth, vl, lngth-1);
    root->right = dfs(pre, vin, pl+lngth+1, pr, map[val]+1, vr);
    return root;
}

//JZ26 树的子结构
bool HasSubtree(TreeNode* pRoot1, TreeNode* pRoot2)
{
    if (!pRoot1 || !pRoot2) return false;
    if (dfs(pRoot1, pRoot2)) return true;
    return HasSubtree(pRoot1->left, pRoot2) || HasSubtree(pRoot1->right, pRoot2);
}
bool dfs(TreeNode* pRoot1, TreeNode* pRoot2) {
    if (!pRoot2) return true;
    if (!pRoot1 || pRoot1->val != pRoot2->val) return false;
    return dfs(pRoot1->left, pRoot2->left) && dfs(pRoot1->right, pRoot2->right);
}

//JZ32 从上到下打印二叉树
vector<int> PrintFromTopToBottom(TreeNode* root) {
    vector<int> res;
    if (!root) return res;
    queue<TreeNode*> q;
    q.push(root);
    while (q.size())
    {
        res.push_back(q.front()->val);
        if (q.front()->left) q.push(q.front()->left);
        if (q.front()->right) q.push(q.front()->right);
        q.pop();
    }
    return res;
}

//JZ33 二叉搜索树的后序遍历序列
 vector<int> s;
bool VerifySquenceOfBST(vector<int> sequence) {
    s = sequence;
    if (s.empty()) return false;
    return dfs(0, s.size()-1);
}
bool dfs(int l, int r) {
    if (l > r) return true;
    int root = s[r];
    int k = l;
    while (s[k] < root) ++k;
    for (int i=k; i<r; ++i) 
    {
        if (s[i] < root) return false;
    }
    return dfs(l, k-1) && dfs(k, r-1);
}

//JZ34 二叉树中和为某一值的路径
vector<vector<int>> res;
vector<int> path;
vector<vector<int> > FindPath(TreeNode* root,int expectNumber) {
    if (!root) return res;
    dfs(root, expectNumber);
    return res;
}
void dfs(TreeNode* root, int sum) {
    if (!root) return;
    path.push_back(root->val);
    sum = sum - root->val;
    dfs(root->left, sum);
    dfs(root->right, sum);
    if (sum == 0 && !root->left && !root->right) 
    {
        res.push_back(path);
    }
    path.pop_back();
}

//JZ36 二叉搜索树与双向链表
TreeNode* Convert(TreeNode* pRoot)
{
    if (!pRoot) return nullptr;
    return dfs(pRoot).first;
}
pair<TreeNode*, TreeNode*> dfs(TreeNode* pRoot)
{
    if (!pRoot->left && !pRoot->right) return {pRoot, pRoot};
    if (pRoot->left && pRoot->right)
    {
        auto lside = dfs(pRoot->left), rside = dfs(pRoot->right);
        lside.second->right = pRoot, pRoot->left = lside.second;
        rside.first->left = pRoot, pRoot->right = rside.first;
        return {lside.first, rside.second};
    }
    if (pRoot->left)
    {
        auto lside = dfs(pRoot->left);
        lside.second->right = pRoot, pRoot->left = lside.second;
        return {lside.first, pRoot};

    }
    if (pRoot->right)
    {
        auto rside = dfs(pRoot->right);
        rside.first->left = pRoot, pRoot->right = rside.first;
        return {pRoot, rside.second};
    }
}

//JZ79 判断是不是平衡二叉树
bool res = true;
bool IsBalanced_Solution(TreeNode* pRoot) {
    dfs(pRoot);
    return res;
}
int dfs(TreeNode* pRoot)
{
    if (!pRoot) return 0;
    int left = dfs(pRoot->left), right = dfs(pRoot->right);
    if (abs(left-right) > 1) res = false;
    return max(left, right) + 1;
}

//JZ8 二叉树的下一个结点
TreeLinkNode* GetNext(TreeLinkNode* pNode)
{
    if (pNode->right) {
        auto p = pNode->right;
        while (p->left) p = p->left;
        return p;
    }
    while (pNode->next) {
        if (pNode == pNode->next->left) return pNode->next;
        pNode = pNode->next;
    }
    return nullptr;
}

//JZ28 对称的二叉树
bool isSymmetrical(TreeNode* pRoot)
{
    return !pRoot || dfs(pRoot->left, pRoot->right);
}
bool dfs(TreeNode* left, TreeNode* right) {
    if (!left || !right) return !left && !right;
    if (left->val == right->val) return dfs(left->left, right->right) && dfs(left->right, right->left);
    return false;
}

//JZ78 把二叉树打印成多行
vector<vector<int> > Print(TreeNode* pRoot) {
    vector<vector<int>> res;
    vector<int> cur;
    if (!pRoot) return res;
    queue<TreeNode*> q;
    q.push(pRoot);
    while (q.size())
    {
        int n = q.size();
        for (int i = 0; i < n; i++)
        {
            auto t = q.front();
            q.pop();
            if (t->left) q.push(t->left);
            if (t->right) q.push(t->right);
            cur.push_back(t->val);
        }
        res.push_back(cur);
        cur.clear();
    }
    return res;
}
//JZ37 序列化二叉树
char* Serialize(TreeNode *root) { 
    string ans;
    dfs1(root, ans);
    char * strc = new char[strlen(ans.c_str())+1];
    strcpy(strc, ans.c_str());
    return strc;
    
}
void dfs1(TreeNode *root, string &ans)
{
    if (!root)
    {
        ans += '#';
        return;
    }
    ans += to_string(root->val) + '!';
    dfs1(root->left, ans);
    dfs1(root->right, ans);
}
TreeNode* Deserialize(char *str) {
    int u = 0;
    return dfs2(str, u);

}
TreeNode* dfs2(char *str, int &u)
{
    if (str[u] == '#') 
    {u++;
        return nullptr;}
    bool flag = false;
    if (str[u] == '-') u++, flag = true;
    int num = 0;
    while (str[u] != '!')
    {
        num = num*10 + str[u] - '0';
        u++;
    }
    u++;
    if (flag) num *= -1;
    auto root = new TreeNode(num);
    root->left = dfs2(str, u);
    root->right = dfs2(str, u);
    return root;
}
