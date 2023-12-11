#include "gomoku.h"

// 另一个判断是不是禁手, 供活三判断递归中使用的函数, 仅用于计算活四
int judge_legal_another(int row_place, int col_place, int board[BOARD_SIZE][BOARD_SIZE])
{
    /*
        0~7：从右开始, 逆时针旋转, 代表的八个方向。
        row_place 和 col_place 是判断是不是禁手的位置
    */

    // 定义函数内可以不停变动的行和列
    int row, col;

    // nearby连接数; jump跳跃数记录; neighbor跳跃后连接数; end最头上是什么棋子或无棋子
    int nearby[8], jump[8], neighbor[8], end[8];

    // 中间（落子的位置）直线上有几个相连的棋子
    int middle[4];

    for (int i = 0; i < DIRECTION; i++) // 去判断所有8个不同的方向, 读入数据
    {
        // 这些都是复位的操作
        row = row_place;
        col = col_place;
        nearby[i] = 0;
        jump[i] = 0;
        neighbor[i] = 0;
        end[i] = 0;

        // 一开始先挪两下位置
        row += row_sign[i];
        col += col_sign[i];

        while (judge_in_bounds(row, col) == IN_BOUND && jump[i] <= 1)
        {
            if (board[row][col] == SPARE)      // 没有棋子
                jump[i]++;                         // 跳跃+1
            else if (board[row][col] == BLACK) // 有黑棋
            {
                if (jump[i] == 0)  // 还没有跳跃过
                    nearby[i]++;   // 相邻+1
                else               // 有跳跃过
                    neighbor[i]++; // 跳跃后相邻+1
            }
            else       // 有白棋
                break; // 直接停

            // 去到下一个位置
            row += row_sign[i];
            col += col_sign[i];
        }
        end[i] = judge_in_bounds(row, col) ? board[row][col] : WHITE; // 录入尽头的棋子类型, 分有没有到头两类
    }

    for (int i = 0; i < HALF_DIRECTION; i++)
        middle[i] = nearby[i] + nearby[i + 4] + 1; // 把中间的棋子计算出来

    int special_3 = 0;    // 活三的个数记录
    int special_4 = 0;    // 活四的个数记录
    int special_long = 0; // 长连的个数记录
    int special_4_temp;   // 暂时存一下活四冲四/五子连珠的个数
    int live_4_exist = 0;     // 计算活四数量

    //  递归用的！！！

    for (int i = 0; i < HALF_DIRECTION; i++)
    {
        if (middle[i] == 5) // 五子连珠直接取胜
            return 0;
        else if (middle[i] > 5) // 长连做记录, 不再判断4和3
            ++special_long;
        if (special_long == 0) // 有长连记录无需判断三四
        {
            // 跑一下判断是否有活四、冲四的函数
            special_4_temp = judge_4(neighbor[i], neighbor[i + HALF_DIRECTION], jump[i], jump[i + 4], middle[i]);
            if (special_4_temp == 4) // 如果有活四的话
                live_4_exist = 1;
            if (special_4_temp) // 如果有四的话, 把暂存放到存储位置
                special_4 += special_4_temp % 3;
            else // 如果没有四, 判断有没有活三, 直接加进去
                special_3 += judge_3(nearby[i], nearby[i + 4], neighbor[i], neighbor[i + HALF_DIRECTION], jump[i], jump[i + 4], row_place, col_place, i, board);
        }
    }
    if (special_long || special_4 >= 2 || special_3 >= 2)
        return 0;
    if (live_4_exist)
        return 1; // 只有这样才能算是一个有用的活四, 原来的就是一个活三
    return 0;
}
