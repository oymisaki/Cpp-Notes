#include <vector>

using namespace std;

int ON2lengthOfLIS(vector<int>& nums)
{
    const int size = nums.size();
    if(size == 0)
        return 0;

    vector<int> dp(size, 1);
    int res = 1;

    for(int i = 1; i<size; ++i)
    {
        for(int j = 0; j < i; ++j)
        {
            if(nums[i] > nums[j])
                dp[i] = max(dp[i], dp[j]+1);
        }
        
        res = max(res, dp[i]);
    }

    return res;
}

int ONLOGNlengthOfLIS(vector<int>& nums)
{
    vector<int> res;

    for(int i = 0; i<nums.size(); ++i)
    {
        auto it = lower_bound(res.begin(), res.end(), nums[i]);
        if(it == res.end())
            res.push_back(nums[i]);
        else
            *it = nums[i];
    }

    return res.size();
}


