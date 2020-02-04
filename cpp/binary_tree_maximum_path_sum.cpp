#include <iostream>
#include<bits/stdc++.h> 
using namespace std;

class TreeNode
{
public:
    int val;
    TreeNode * left = nullptr;
    TreeNode * right = nullptr;
};

class Solution {
    int sum;
public:
    int maxPathSum(TreeNode* root) {
        sum=INT_MIN;
        help(root);
        return sum;
    }
    
    /*** return the max-value-ended-at-root-node ***/
    int help(TreeNode* root){
        if(!root)   return 0;
        int left = max(0, help(root->left));
        int right = max(0, help(root->right));
        /*** key parts : embedding the max-value-find in the recursion process ***/
        sum = max(sum, left+right+root->val);
        /*** get the max-value-ended-at-root ***/
        return max(left, right)+root->val;
    }
};