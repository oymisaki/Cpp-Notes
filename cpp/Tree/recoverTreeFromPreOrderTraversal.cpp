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
    TreeNode* recoverFromPreorder(string S) {
        vector<pair<int, TreeNode *>> s;
        TreeNode * root = nullptr;
        
        
        for(int i = 0; i < S.size();)
        {
            int level = 0;
            for(;S[i] == '-'; i++)
                level ++;
                            
            int val = 0;
            for(;i < S.size() && S[i] != '-'; i++)
                val = val * 10 + S[i] - '0';

            TreeNode * node = new TreeNode(val);
            if(!root)
                root = node;
            
            while(!s.empty() && s.back().first > level)
                s.pop_back();

            if(!s.empty() && s.back().first == level)
            {
                s.pop_back();
                s.back().second -> right = node;
            }
            else if(!s.empty())
                s.back().second -> left = node;

            s.emplace_back(level, node);
        }
        
        return root;
    }
};