#include <vector>
#include <stack>

using namespace std;

class TreeNode
{
public:
    int val;
    TreeNode * left = nullptr;
    TreeNode * right = nullptr;
};

vector<int> postorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        if(root)
        {
            todo.push(root);
            root = root->left;
        }
        else
        {
            TreeNode * node = todo.top();
            if(node -> right && last != node -> right)
            {
                root = node -> right;
            }
            else
            {
                nodes.push_back(node -> val);
                last = node;
                todo.pop();
            }
        }
    }
}

vector<int> preorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        if(root)
        {
            nodes.push_back(root->val);
            if(root -> right)
            {
                todo.push(root->right);
            }
            root = root -> left;
        }
        else
        {
            root = todo.top();
            todo.pop();
        }
    }
}

vector<int> inorderTraversal(TreeNode* root)
{
    vector<int> nodes;
    stack<TreeNode *> todo;
    TreeNode* last = nullptr;

    while(root || !todo.empty())
    {
        while(root)
        {
            todo.push(root);
            root = root -> left;
        }

        root = todo.top();
        todo.pop();

        nodes.push_back(root->val);
        root = root -> right;
    }
}
