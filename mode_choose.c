#include "gomoku.h"

// 选取游戏模式
int mode_choose(int mode_or_first)
{
    char mode;
    int length;
    char input_string[MAXLEN];

    if (mode_or_first == MODE)
        printf("请你选择游戏模式，输入1表示人人对战，2表示人机对战，输入0则结束游戏：");
    else if (mode_or_first == FIRST)
        printf("请你选择您的先后手顺序，输入1玩家为执黑先行，2玩家执白后行，输入0则结束游戏：");
    else if (mode_or_first == RETRACT)
        printf("请选择悔棋的发起者，输入1表示黑棋，2表示白棋，输入0则结束游戏：");
    else
    {
        printf("mode_choose错误参数！\n");
        return -1;
    }

    // 永远不停地循环输入, 直到输入退出游戏或者
    while (1)
    {
        length = get_full_line(input_string, MAXLEN); // 输入一行, 返回字符串长度 (有EOF则返回-1)
        mode = input_string[0];                       // 输入的游戏模式应当是字符串的第一个字符

        // 长度为-1表示读到了stdin中的EOF. 另一种情况是读到了 "0\n"
        if (length == -1 || (length == 2 && mode == '0'))
        {
            printf("退出游戏。\n");
            return GAME_STOP;
        }
        // 长度2是输入一个字符和回车的时候, 应该有的字符串长度. 这时候字符串是 "1\n" 或者 "2\n"
        if (length == 2 && (mode == '1' || mode == '2'))
        {
            if (mode == '1')
                return 1;
            else if (mode == '2')
                return 2;
        }
        printf("输入有误，请您重新选择：");
    }
    printf("mode_choose函数发生错误！\n");
    return -1;
}
