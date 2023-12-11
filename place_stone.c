#include "gomoku.h"

// 负责落子的函数
int place_stone(int player_turn, int round, int game_mode)
{
    char col_input;        // 输入的落子的列
    int row_input;         // 输入的落子的行
    int row;               // 落子的行
    int col;               // 落子的行
    char input[MAX_INPUT]; // 所有输入的字符串
    char c;                // 一个缓存char
    char s[MAX_INPUT];     // 一个缓存字符串
    int length;            // 字符串的长度
    int regert_succeed;    // 悔棋是否成功
    int i;

    printf("请输入下一步要走的位置 如h08、8H等："); // 打印提示

    length = get_full_line(input, MAX_INPUT);

    // 把所有字母都变成小写的字母
    for (i = 0; i < length; i++)
        if (isupper(input[i]))            // 检查是不是大写字母
            input[i] = tolower(input[i]); // 转换为小写字母

    c = input[length - 1];

    // 最后一个字符是文档终止符
    if (length == -1)
    {
        printf("退出游戏。\n");
        return END_GAME;
    }

    // 最后一个字符不是'\n'
    if (c != '\n')
    {
        printf("输入超出上限。\n"); // 报错
        return INPUT_WRONG;
    }

    // 有EOF, end, quit, stop: 停止输入, 结束游戏
    if (strstr(input, "end") != NULL || strstr(input, "quit") != NULL || strstr(input, "stop") != NULL)
    {
        printf("退出游戏。\n");
        return END_GAME;
    }

    // 有restart: 重新开始游戏
    if (strstr(input, "restart") != NULL)
    {
        printf("重新开始游戏。\n");
        return RESTART_GAME;
    }

    // 有regret, back, retract: 悔棋
    if (strstr(input, "regret") != NULL || strstr(input, "back") != NULL || strstr(input, "retract") != NULL)
    {
        regert_succeed = retract_stone(player_turn, round, game_mode);
        if (regert_succeed == RETRACT_ONCE || regert_succeed == RETRACT_TWICE)
            printf("悔棋成功。\n");
        return regert_succeed;
    }

    // isalpha是<ctype.h>的库函数, 判断一个char是不是字母
    if (isalpha(input[0])) // 第0个字符是字母
    {
        if (isdigit(input[1]) && isdigit(input[2]) && length == 4) // 注意, '\n'也会被存入字符串中
            row_input = (input[1] - '0') * 10 + (input[2] - '0');  // 第1 2个字符读到行里
        else if (isdigit(input[1]) && length == 3)                 // 同理
            row_input = input[1] - '0';                            // 第1个字符读到行里
        else
        {
            printf("输入有误。(col = input[0])\n"); // 报错
            return INPUT_WRONG;
        }
        col_input = input[0]; // 第0个字符读到列里
    }
    else if (isalpha(input[1])) // 第1个字符是字母
    {
        if (isdigit(input[0]) && length == 3)
            row_input = input[0] - '0'; // 第0个字符读到行里
        else
        {
            printf("输入有误。(col = input[1])\n"); // 报错
            return INPUT_WRONG;
        }
        col_input = input[1]; // 第1个字符读到列里
    }
    else if (isalpha(input[2])) // 第2个字符是字母
    {
        if (isdigit(input[0]) && isdigit(input[1]) && length == 4)
            row_input = (input[0] - '0') * 10 + (input[1] - '0'); // 第0 1个字符读到行里
        else
        {
            printf("输入有误。(col = input[2])\n"); // 报错
            return INPUT_WRONG;
        }
        col_input = input[2]; // 第2个字符读到列里
    }
    else
    {
        printf("输入有误。(no alpha detected among first three characters)\n"); // 报错
        return INPUT_WRONG;
    }

    row = BOARD_SIZE - row_input;
    col = col_input - 'a';

    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) // 落子的位置在棋盘范围之内
    {
        if (inner_board_now[row][col]) // 落子的位置已经有子
        {
            printf("落子的位置已经有子。\n");
            return INPUT_WRONG;
        }
    }
    else
    {
        printf("落子的位置在棋盘范围之外。\n"); // 落子的位置在棋盘范围之外
        return INPUT_WRONG;
    }
    record_position(round, player_turn, row, col);
    return POSITION_AVAILABLE;
}
