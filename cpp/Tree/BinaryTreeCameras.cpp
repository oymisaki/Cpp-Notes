// greedy


#include <vector>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
    int res = 0;
    
    int minCameraCover(TreeNode* root) {
        if(dfs(root) == 0)
            res++;
        return res;
    }
    
    int dfs(TreeNode* root)
    {
        if(!root)
            return 2;
        
        int left = dfs(root->left);
        int right = dfs(root->right);
        
        if(left == 0 || right == 0)
        {
            res++;
            return 1;
        }
        else if(left == 1 || right == 1)
            return 2;
        else
            return 0;
    }
};