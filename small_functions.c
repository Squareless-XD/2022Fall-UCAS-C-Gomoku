#include "gomoku.h"

// 求最大值
ll max(ll a, ll b)
{
    return a > b ? a : b;
}

// 求最小值
ll min(ll a, ll b)
{
    return a < b ? a : b;
}

// 判断是否越界的函数
int judge_in_bounds(int row, int col)
{
    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE)
        return IN_BOUND;
    return BEYOND_BOUND;
}

// 清零各个全局变量
void clear_variables(void)
{
    int i;

    memset(tuple_set_hori, 0, sizeof(tuple_set_hori));
    memset(tuple_set_topr, 0, sizeof(tuple_set_topr));
    memset(tuple_set_vert, 0, sizeof(tuple_set_vert));
    memset(tuple_set_topl, 0, sizeof(tuple_set_topl));
    memset(point_hori, 0, sizeof(point_hori));
    memset(point_topr, 0, sizeof(point_topr));
    memset(point_vert, 0, sizeof(point_vert));
    memset(point_topl, 0, sizeof(point_topl));

    col_now_global = 0;
    row_now_global = 0;
    row_black_global = 0;
    col_black_global = 0;
    row_white_global = 0;
    col_white_global = 0;
    retract_times_left_black = 0;
    retract_times_left_white = 0;

    for (i = 0; i < MAX_RETRACT; i++)
    {
        row_black_global_back[i] = 0;
        col_black_global_back[i] = 0;
        row_white_global_back[i] = 0;
        col_white_global_back[i] = 0;
    }

    for (i = 0; i < MAX_RECORD_SIZE; i++)
    {
        game_record[i].row = 0;
        game_record[i].col = 0;
    }

    first_layer_max_used_width = 0;
}
