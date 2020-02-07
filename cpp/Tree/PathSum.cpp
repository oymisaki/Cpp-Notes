
#include <vector>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// dfs回溯
class Solution
{
public:
    vector<int> vec;
    vector<vector<int>> res;

    vector<vector<int>> pathSum(TreeNode *root, int sum)
    {

        dfs(root, sum);
        return res;
    }

    // dfs 的回溯
    void dfs(TreeNode *root, int sum)
    {
        if (!root)
            return;

        if (root && !root->left && !root->right && sum == root->val)
        {
            vec.push_back(root->val);
            res.push_back(vec);
            vec.pop_back();
            return;
        }

        vec.push_back(root->val);
        sum -= root->val;
        dfs(root->left, sum);
        dfs(root->right, sum);
        vec.pop_back();
    }
};