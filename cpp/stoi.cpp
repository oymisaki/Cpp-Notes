
#include <iostream>
using namespace std;

int StringToInt(const char *str);
int StringToIntCore(const char *str, bool minus); //字符串转整型核心函数

int StringToInt(const char *str)
{

    int num;
    if (str != NULL && *str != '\0') //str为空指针或空字符串返回零
    {
        //处理正负号
        bool minus = false;
        if (*str == '+')
            str++;
        else if (*str == '-')
        {
            str++;
            minus = true;
        }
        if (*str != '\0') //处理输入只有正负号情况
        {
            num = StringToIntCore(str, minus);
        }
    }
    else
        throw std::invalid_argument("Invalid String Input");

    return num;
}

int StringToIntCore(const char *str, bool minus)
{
    long long num = 0;
    while (*str != '\0')
    {
        if (*str >= '0' && *str <= '9') //转换‘0’~'9'之间的字符
        {
            int flag = minus ? -1 : 1; //条件运算符
            num = num * 10 + flag * (*str - '0');
            //判断num是否溢出
            if ((!minus && num > 0x7FFFFFFF) || (minus && num < (signed int)0x80000000)) 
            //(signed int)0x80000000为int 型的最小负数，必须加signed int
                throw std::overflow_error("Integer Overflow");

            str++; //向后移动一位
        }
        else
        {
            throw std::invalid_argument("Invalid String Input");
        }
    }

    return (int)num;
}

int main()
{
    cout<<StringToInt("111111111111111111")<<endl;
    return 0;
}
