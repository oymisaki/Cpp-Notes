#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8};
    std::rotate(vec.begin() + 1, vec.end() - 1, vec.end());

    for (int i = 0; i < 4; ++i)
        cout << vec[i] << ' ';
    cout << endl;
    for (int i = 7; i > 3; --i)
        cout << vec[i] << ' ';
    cout << endl;

    std::rotate(vec.begin() + 1, vec.end() - 1, vec.end());

    for (int i = 0; i < 4; ++i)
        cout << vec[i] << ' ';
    cout << endl;
    for (int i = 7; i > 3; --i)
        cout << vec[i] << ' ';
    cout << endl;

    return 0;
}