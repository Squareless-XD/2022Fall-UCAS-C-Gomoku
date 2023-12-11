#include "gomoku.h"

// 获取stdin中的一行输入, 并且将stdin后续的字符清除, 直至'\n'或EOF. 返回字符串的长度, 若读到了EOF, 返回-1
int get_full_line(char s[], int lim)
{
    int c, i;
    for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i)
        s[i] = c;
    if (c == '\n')
    {
        s[i] = c;
        ++i;
    }
    else if (c != EOF) // 新增一段判断, 用以读完整行, 这样c必定是'\n'或者EOF之一.
    {
        while ((c = getchar()) != EOF && c != '\n')
            ;
    }
    s[i] = '\0';
    return (c == EOF) ? -1 : i; // 如果c是EOF, 那么返回长度为-1. 否则返回长度为i, 这是strlen(s)的结果.
}
