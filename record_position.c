#include "gomoku.h"

// 将玩家/机器的落子记录下来的一系列操作, 得到的很多结果用的都是全局变量
void record_position(int round, int player_turn, int row, int col)
{
    int i;
    row_now_global = row; // 得到了行的结果
    col_now_global = col; // 得到了列的结果

    if (player_turn == BLACK) // 写入刚才下的黑子
    {
        // 把之前存储的棋子不断向内存
        for (i = MAX_RETRACT - 1; i > 0; i--)
        {
            row_black_global_back[i] = row_black_global_back[i - 1];
            col_black_global_back[i] = col_black_global_back[i - 1];
        }
        row_black_global_back[0] = row_black_global;
        col_black_global_back[0] = col_black_global;
        row_black_global = row_now_global;
        col_black_global = col_now_global;

        // 增加可悔棋次数
        if (retract_times_left_black < MAX_RETRACT)
            ++retract_times_left_black;
    }
    else if (player_turn == WHITE) // 写入刚才下的白子
    {
        for (i = MAX_RETRACT - 1; i > 0; i--)
        {
            row_white_global_back[i] = row_white_global_back[i - 1];
            col_white_global_back[i] = col_white_global_back[i - 1];
        }
        row_white_global_back[0] = row_white_global;
        col_white_global_back[0] = col_white_global;
        row_white_global = row_now_global;
        col_white_global = col_now_global;
        if (retract_times_left_white < MAX_RETRACT)
            ++retract_times_left_white;
    }

    // 记录棋谱
    game_record[round].row = row_now_global;
    game_record[round].col = col_now_global;

    place_stone_player(row_now_global, col_now_global, player_turn); // 更改打分的六元组集合

    // 可悔棋次数若不足MAX_RETRACT, 则+1
}
