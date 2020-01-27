#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;

string longestPalindrome(string s) {
    const int size = s.size();

    int max_len = 1;
    int b = 0, e = 0;

    for(int i = 0; i < size; ++i)
    {
        for(int r: {i, i+1})
        {
            int l = i;
            while(l > 0 && r < size && s[l] == s[r])
                --l; ++r;
            
            if(r - l - 1 > max_len)
            {
                b = l + 1;
                e = r - 1;
                max_len = r - l - 1;
            }
        }
    }
}

int manacher(string s)
{
    std::stringstream buffer;
    buffer << "#";
    std::copy(s.begin(), s.end(), std::ostream_iterator<char>(buffer, "#"));
    string T = buffer.str();

    vector<int> P(T.size(), 1);
    int C = 0, R = -1, rad; 
    for (int i = 0; i < T.length(); ++i) {
        if (i <= R) {
            rad = std::min(P[2*C-i], R-i); 
        } else {
            rad = 0; 
        }
        // Try to extend
        while (i+rad < T.length() && i-rad >= 0 && T[i-rad] == T[i+rad]) {
            rad++; 
        }
        P[i] = rad - 1;
        if (i + P[i] > R) {
            C = i; 
            R = i + P[i]; 
        }
    }

    for(auto i: P)
        cout<<i<<" ";

    return *std::max_element(P.begin(), P.end());
}

int main()
{
    string s = "abaababccdddcc";

    cout<<manacher(s)<<endl;
    return 0;
}