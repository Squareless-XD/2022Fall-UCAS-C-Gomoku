#include "gomoku.h"

// 悔棋
int retract_stone(int player_turn, int round, int game_mode)
{
    int retract_player;

    if (round <= 1) // 黑棋本来就啥都没下
    {
        printf("轮次过低，无法悔棋！\n");
        return INPUT_WRONG;
    }

    // 人人模式选择是谁要悔棋
    if (game_mode == PLAYER_PLAYER)
        retract_player = mode_choose(RETRACT);
    // 人机模式自动选择为玩家悔棋 (因为机器落子的时候玩家啥都干不了)
    else
        retract_player = player_turn;

    if (retract_player == WHITE)
    {
        if (round == 2) // 白棋本来就啥都没下
        {
            printf("轮次过低，无法悔棋！\n");
            return INPUT_WRONG;
        }
        if (retract_times_left_white == 0 || (player_turn == WHITE && retract_times_left_black == 0)) // 不够悔棋的了
        {
            printf("悔棋次数用尽！\n");
            return INPUT_WRONG;
        }

        // 下面是可以悔棋时进行的悔棋操作
        if (player_turn == WHITE)
        {
            retract_black();
            retract_white();
            return RETRACT_TWICE;
        }
        else
        {
            retract_white();
            return RETRACT_ONCE;
        }
    }
    else if (retract_player == BLACK)
    {
        if (retract_times_left_black == 0 || (player_turn == BLACK && retract_times_left_white == 0)) // 不够悔棋的了
        {
            printf("悔棋次数用尽！\n");
            return INPUT_WRONG;
        }

        if (player_turn == BLACK)
        {
            retract_white();
            retract_black();
            return RETRACT_TWICE;
        }
        else
        {
            retract_black();
            return RETRACT_ONCE;
        }
    }
    else
    {
        printf("输入有误，请您重新选择！\n");
        return INVALID_INPUT;
    }
    return INVALID_INPUT;
}

// 撤回一个黑子, 操作和落子时的操作类似
void retract_black(void)
{
    int i;
    place_stone_player(row_black_global, col_black_global, SPARE); // 把真棋盘上面的子给去掉
    row_black_global = row_black_global_back[0];
    col_black_global = col_black_global_back[0];
    for (i = 0; i < MAX_RETRACT - 1; i++)
    {
        row_black_global_back[i] = row_black_global_back[i + 1];
        col_black_global_back[i] = col_black_global_back[i + 1];
    }
    row_black_global_back[MAX_RETRACT - 1] = 0;
    col_black_global_back[MAX_RETRACT - 1] = 0;
    --retract_times_left_black;
}

// 撤回一个白子, 操作和落子时的操作类似
void retract_white(void)
{
    int i;
    place_stone_player(row_white_global, col_white_global, SPARE); // 把真棋盘上面的子给去掉
    row_white_global = row_white_global_back[0];
    col_white_global = col_white_global_back[0];
    for (i = 0; i < MAX_RETRACT - 1; i++)
    {
        row_white_global_back[i] = row_white_global_back[i + 1];
        col_white_global_back[i] = col_white_global_back[i + 1];
    }
    row_white_global_back[MAX_RETRACT - 1] = 0;
    col_white_global_back[MAX_RETRACT - 1] = 0;
    --retract_times_left_white;
}
