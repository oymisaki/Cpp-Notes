
#include <iostream>
#include <assert.h>                          //调用assert函数
using namespace std;                         //使用标准命名空间


char * str_cpy(char *dst, const char *src) //dst长度必须大于等于src，否则会出错
{
    assert(dst != NULL && src != NULL); //保持程序鲁棒性
    char *result = dst;
    while (*src != '\0')
    {
        *(dst++) = *(src++);
    }

    *dst = '\0'; //字符串末尾为'\0'字符
    
    return result;
}