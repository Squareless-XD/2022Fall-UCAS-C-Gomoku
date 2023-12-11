#include "gomoku.h"

// 在copy上落子或者取消落子
void place_stone_on_copy(int row, int col, int stone)
{
    int i, j, pattern;
    int temp_r, temp_c;

    // 如果在棋盘外就不进行操作 (在minimax搜索的时候有用, 因为root的点是(-1,-1), 在界外的)
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return;

    // 复制的棋盘
    chessboard_copy[row][col] = stone;

    // 打分用的六元组: 横向
    for (i = max(0, col - TUPLE_NUMB + 1); i < min(TUPLE_SIZE, col + 1); i++)
    {
        temp_c = col - i;
        tuple_copy_hori[row][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_copy_hori[row][temp_c][j];
        }
        point_copy_hori[row][temp_c] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 左下-右上
    for (i = max(max(0, col - TUPLE_NUMB + 1), TUPLE_SIZE - 1 - row); i < min(min(TUPLE_SIZE, col + 1), BOARD_SIZE - row); i++)
    {
        temp_r = row - 5 + i;
        temp_c = col - i;
        tuple_copy_topr[temp_r][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_copy_topr[temp_r][temp_c][j];
        }
        point_copy_topr[temp_r][temp_c] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 纵向
    for (i = max(0, row - TUPLE_NUMB + 1); i < min(TUPLE_SIZE, row + 1); i++)
    {
        temp_r = row - i;
        tuple_copy_vert[temp_r][col][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_copy_vert[temp_r][col][j];
        }
        point_copy_vert[temp_r][col] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 左上-右下
    for (i = max(max(0, col - TUPLE_NUMB + 1), row - TUPLE_NUMB + 1); i < min(min(TUPLE_SIZE, col + 1), row + 1); i++)
    {
        temp_r = row - i;
        temp_c = col - i;
        tuple_copy_topl[temp_r][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_copy_topl[temp_r][temp_c][j];
        }
        point_copy_topl[temp_r][temp_c] = get_grade_for_tuple(pattern);
    }
}

// 在真正的棋盘上落子或者取消落子
void place_stone_player(int row, int col, int stone)
{
    int i, j, pattern;
    int temp_r, temp_c;

    // 复制的棋盘
    inner_board_now[row][col] = stone;

    // 打分用的六元组: 横向
    for (i = max(0, col - TUPLE_NUMB + 1); i < min(TUPLE_SIZE, col + 1); i++)
    {
        temp_c = col - i;
        tuple_set_hori[row][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_set_hori[row][temp_c][j];
        }
        point_hori[row][temp_c] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 左下-右上
    for (i = max(max(0, col - TUPLE_NUMB + 1), TUPLE_SIZE - 1 - row); i < min(min(TUPLE_SIZE, col + 1), BOARD_SIZE - row); i++)
    {
        temp_r = row - 5 + i;
        temp_c = col - i;
        tuple_set_topr[temp_r][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_set_topr[temp_r][temp_c][j];
        }
        point_topr[temp_r][temp_c] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 纵向
    for (i = max(0, row - TUPLE_NUMB + 1); i < min(TUPLE_SIZE, row + 1); i++)
    {
        temp_r = row - i;
        tuple_set_vert[temp_r][col][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_set_vert[temp_r][col][j];
        }
        point_vert[temp_r][col] = get_grade_for_tuple(pattern);
    }

    // 打分用的六元组: 左上-右下
    for (i = max(max(0, col - TUPLE_NUMB + 1), row - TUPLE_NUMB + 1); i < min(min(TUPLE_SIZE, col + 1), row + 1); i++)
    {
        temp_r = row - i;
        temp_c = col - i;
        tuple_set_topl[temp_r][temp_c][i] = stone;
        pattern = 0;
        for (j = 0; j < TUPLE_SIZE; j++) // 将每一个六元组按照八进制存储
        {
            pattern <<= 3;
            pattern += tuple_set_topl[temp_r][temp_c][j];
        }
        point_topl[temp_r][temp_c] = get_grade_for_tuple(pattern);
    }
}
