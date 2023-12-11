#include "gomoku.h"

// 判断玩家是否已经取胜
int judge_winning(int player_turn, int row_place, int col_place, int board[BOARD_SIZE][BOARD_SIZE])
{
    int row;    // 行个数
    int col;    // 列个数
    int nearby; // 连接个数

    for (int i = 0; i < HALF_DIRECTION; i++)
    {
        nearby = 1;

        // 复位
        row = row_place + row_sign[i];
        col = col_place + col_sign[i];

        while ((board[row][col] == player_turn) && judge_in_bounds(row, col) == IN_BOUND)
        {
            nearby++;           // 连接的子数+1
            row += row_sign[i]; // 行到下一个位置
            col += col_sign[i]; // 列到下一个位置
        }

        // 复位
        row = row_place - row_sign[i];
        col = col_place - col_sign[i];

        while ((board[row][col] == player_turn) && judge_in_bounds(row, col) == IN_BOUND)
        {
            nearby++;           // 连接的子数+1
            row -= row_sign[i]; // 行到下一个位置
            col -= col_sign[i]; // 列到下一个位置
        }

        // 白棋至少五子连珠直接取胜, 黑棋五子连珠取胜
        if (nearby >= 5 && player_turn == WHITE || nearby == 5 && player_turn == BLACK)
            return GAME_OVER;
    }
    return GAME_CONTINUE;
}
