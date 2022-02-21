//Leetcode 104 二叉树的最大深度
//采用二叉树框架思维，前序代表进入节点，depth+1，中序代表离开左子树将要进入右子树，后序代表离开节点；
int res = 0, depth = 0;
int maxDepth(TreeNode* root) {
    traverse(root);
    return res;
}

void traverse(TreeNode *root) {
    if (!root) {
        res = max(res, depth);
        return;
    }
    depth++;
    traverse(root->left);
    traverse(root->right);
    depth--;
}

//利用分解问题的思路，采用递归实现
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    int maxLeft = maxDepth(root->left);
    int maxRight = maxDepth(root->right);
    int res = max(maxLeft, maxRight) + 1;
    return res;
}

//类似的，二叉树的前序遍历
List<Integer> res = new LinkedList<>();

// 返回前序遍历结果
List<Integer> preorderTraverse(TreeNode root) {
    traverse(root);
    return res;
}

// 二叉树遍历函数
void traverse(TreeNode root) {
    if (root == null) {
        return;
    }
    // 前序位置
    res.add(root.val);
    traverse(root.left);
    traverse(root.right);
}

// 采用分解问题的思维，定义：输入一棵二叉树的根节点，返回这棵树的前序遍历结果
List<Integer> preorderTraverse(TreeNode root) {
    List<Integer> res = new LinkedList<>();
    if (root == null) {
        return res;
    }
    // 前序遍历的结果，root.val 在第一个
    res.add(root.val);
    // 利用函数定义，后面接着左子树的前序遍历结果
    res.addAll(preorderTraverse(root.left));
    // 利用函数定义，最后接着右子树的前序遍历结果
    res.addAll(preorderTraverse(root.right));
}

//Leetcode 543
//直观解法，利用遍历二叉树的深度来求直径，但慢
// 记录最大直径的长度
int maxDiameter = 0;

public int diameterOfBinaryTree(TreeNode root) {
    // 对每个节点计算直径，求最大直径
    traverse(root);
    return maxDiameter;
}

// 遍历二叉树
void traverse(TreeNode root) {
    if (root == null) {
        return;
    }
    // 对每个节点计算直径
    int leftMax = maxDepth(root.left);
    int rightMax = maxDepth(root.right);
    int myDiameter = leftMax + rightMax;
    // 更新全局最大直径
    maxDiameter = Math.max(maxDiameter, myDiameter);
    
    traverse(root.left);
    traverse(root.right);
}

// 计算二叉树的最大深度
int maxDepth(TreeNode root) {
    if (root == null) {
        return 0;
    }
    int leftMax = maxDepth(root.left);
    int rightMax = maxDepth(root.right);
    return 1 + Math.max(leftMax, rightMax);
}

//更优的解法
int res = 0;
int diameterOfBinaryTree(TreeNode* root) {
    maxDepth(root);
    return res;
}
int maxDepth(TreeNode *root) {
    if (!root) return 0;
    int maxLeft = maxDepth(root->left);
    int maxRight = maxDepth(root->right);
    res = max(res, maxLeft + maxRight);
    return max(maxRight,maxLeft) + 1;
}

//层序遍历
// 输入一棵二叉树的根节点，层序遍历这棵二叉树
void levelTraverse(TreeNode root) {
    if (root == null) return;
    Queue<TreeNode> q = new LinkedList<>();
    q.offer(root);

    // 从上到下遍历二叉树的每一层
    while (!q.isEmpty()) {
        int sz = q.size();
        // 从左到右遍历每一层的每个节点
        for (int i = 0; i < sz; i++) {
            TreeNode cur = q.poll();
            // 将下一层节点放入队列
            if (cur.left != null) {
                q.offer(cur.left);
            }
            if (cur.right != null) {
                q.offer(cur.right);
            }
        }
    }
}

//Leetcode 226 翻转二叉树
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;
    invertTree(root->left);
    invertTree(root->right);
    return root;
}

//Leetcode 116 填充每个节点的下一个右侧节点指针
Node* connect(Node* root) {
    if (!root) return nullptr;
    connectTwoNode(root->left, root->right);
    return root;
}

void connectTwoNode(Node *left, Node *right) {
    if (!left || !right) return;
    left->next = right;
    connectTwoNode(left->left, left->right);
    connectTwoNode(right->left, right->right);
    connectTwoNode(left->right, right->left);
}

//Leetcode 114 二叉树展开为链表
void flatten(TreeNode* root) {
    if (!root) return;
    flatten(root->left);
    flatten(root->right);

    TreeNode *left = root->left;
    TreeNode *right = root->right;

    root->left = nullptr;
    root->right = left;

    TreeNode *head = root;
    while(head->right) {
        head = head->right;
    }
    head->right = right;
}

//Leetcode 654 最大二叉树
TreeNode* constructMaximumBinaryTree(vector<int>& nums) {
    return build(nums, 0, nums.size()-1);
}

TreeNode* build(vector<int>& nums, int left, int right) {
    if (left > right) return nullptr;
    int maxVal = INT_MIN, index = -1;
    for (int i=left; i<=right; i++) {
        if (nums[i] > maxVal) {
            maxVal = nums[i];
            index = i;
        }
    }
    TreeNode *root = new TreeNode(maxVal);
    root->left = build(nums, left, index-1);
    root->right = build(nums, index+1, right);
    return root;
}

//Leetcode 105 从前序和中序遍历构造二叉树
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    return build(preorder, inorder, 0, preorder.size()-1, 0, inorder.size()-1);
}

TreeNode* build(vector<int>& preorder, vector<int>& inorder, int pl, int pr, int il, int ir) {
    if (pl > pr) return nullptr;
    int val = preorder[pl];
    int index = 0;
    for (int i=il; i <= ir; i++) {
        if (inorder[i] == val) {
            index = i;
            break;
        }
    } 
    int lsize = index - il;
    TreeNode *root = new TreeNode(val);
    root->left = build(preorder, inorder, pl+1, pl+lsize, il, il + lsize-1);
    root->right = build(preorder, inorder, pl+lsize+1, pr, il+lsize+1, ir);
    return root;
}

//Leetcode 106 从中序和后序遍历构造二叉树
TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
    return build(inorder, postorder,  0, inorder.size()-1, 0, postorder.size()-1);
}

TreeNode* build(vector<int>& inorder, vector<int>& postorder, int il, int ir, int pl, int pr) {
    if (il > ir) return nullptr;
    int val = postorder[pr];
    int index = 0;
    for (int i=il; i <= ir; i++) {
        if (inorder[i] == val) {
            index = i;
            break;
        }
    } 
    int lsize = index - il;
    TreeNode *root = new TreeNode(val);
    root->left = build(inorder, postorder, il, il + lsize-1, pl, pl+lsize-1);
    root->right = build(inorder, postorder, il+lsize+1, ir, pl+lsize, pr-1);
    return root;
}

//Leetcode 889 从前序和后序构造二叉树
TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
    return build(preorder, postorder, 0, preorder.size()-1, 0, postorder.size()-1);
}

TreeNode* build(vector<int>& preorder, vector<int>& postorder, int prel, int prer, int postl, int postr)
{
    if (prel > prer) {
        return nullptr;
    }
    if (prel == prer) return new TreeNode(preorder[prel]);
    int val = preorder[prel];
    int leftval = preorder[prel+1];
    int index = 0;
    for (int i=postl; i<postr; i++) {
        if (postorder[i] == leftval) {
            index = i;
            break;
        }
    }
    int leftSize = index - postl + 1;
    TreeNode *root = new TreeNode(val);
    root->left = build(preorder, postorder, prel+1, prel+leftSize, postl, postl+leftSize-1);
    root->right = build(preorder, postorder, prel+leftSize+1, prer, postl+leftSize, postr-1);
    return root;
}

//Leetcode 652 寻找重复子树
unordered_map<string, int> hash;
vector<TreeNode*> res;
vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
    traverse(root);
    return res;
}

string traverse(TreeNode* root) {
    if (!root) return "#";
    string left = traverse(root->left);
    string right = traverse(root->right);
    string sub = left + "," + right + "," + std::to_string(root->val);
    int freq = 0;
    if (hash.count(sub)) {
        freq = hash[sub];
    }
    if (freq == 1) res.push_back(root);
    hash[sub] = freq + 1;
    return sub;
}

//Leetcode 230 二叉搜索树中第k小的元素
int res = 0, rank = 0;
int kthSmallest(TreeNode* root, int k) {
    traverse(root, k);
    return res;
}

void traverse(TreeNode* root, int k) {
    if (!root) return;
    traverse(root->left, k);
    rank++;
    if(rank == k) res = root->val;
    traverse(root->right, k);
}

//Leetcode 538 二叉搜索树转换为累加树
int sum = 0;
TreeNode* convertBST(TreeNode* root) {
    traverse(root);
    return root;
}

void traverse(TreeNode *root) {
    if (!root) return;
    traverse(root->right);
    sum += root->val;
    root->val = sum;
    traverse(root->left);
}

//Leetcode 98 验证二叉搜索树
bool isValidBST(TreeNode* root) {
    return isValidBST(root, nullptr, nullptr);
}

bool isValidBST(TreeNode *root, TreeNode* min, TreeNode* max)
{
    if (!root) return true;
    if (min && min->val >= root->val) return false;
    if (max && max->val <= root->val) return false;
    return isValidBST(root->left, min, root) && isValidBST(root->right, root, max);
}

//Leetcode 700 二叉搜索树中的搜索
TreeNode* searchBST(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (val < root->val) return searchBST(root->left, val);
    if (val > root->val) return searchBST(root->right, val);
    return root;
}

//Leetcode 701 二叉搜索树中的插入操作
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) return new TreeNode(val);
    if (val < root->val) root->left = insertIntoBST(root->left, val);
    if (val > root->val) root->right = insertIntoBST(root->right, val);
    return root;
}

//Leetcode 450 删除二叉搜索树中的节点
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) return nullptr;
    if (root->val == key) {
        if (!root->left) return root->right;
        if (!root->right) return root->left;
        TreeNode *minNode = getMin(root->right);
        root->right = deleteNode(root->right, minNode->val);
        minNode->left = root->left;
        minNode->right = root->right;
        root = minNode;
    }
    else if (root->val < key) root->right = deleteNode(root->right, key);
    else if (root->val > key) root->left =  deleteNode(root->left, key);
    return root;
}

TreeNode* getMin(TreeNode* root) {
    if (!root) return nullptr;
    while (root->left) root = root->left;
    return root;
}