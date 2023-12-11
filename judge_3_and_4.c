#include "gomoku.h"

// 判断是否存在活3的函数
int judge_3(int nearby1, int nearby2, int side1, int side2, int jump1, int jump2, int row, int col, int sign, int board[BOARD_SIZE][BOARD_SIZE])
{
    /*
        side1 jump1 nearby1 '1' nearby2 jump2 side2
        这是所有输入的不同意义.
        jump表示的是空格有几个, side表示最头上有多少个连着的己方子, nearby表示连接处有多少个连着的己方子
        row_sign 和 col_sign 表示的是改网哪个方向做加法, 表明的是方向
    */

    int judge_going_4 = 0;              // 能升格成4
    int middle = nearby1 + nearby2 + 1; // 中间连着的个数, 就是左邻、右邻之和加上1（落子的位置）

    if ((middle + side1) == 3 && jump1)
    {
        row += (nearby1 + 1) * row_sign[sign]; // 没有必要把行列复位哦
        col += (nearby1 + 1) * col_sign[sign];
        if (judge_in_bounds(row, col))
        {
            board[row][col] = BLACK;                               // 先下上黑棋
            judge_going_4 += judge_legal_another(row, col, board); // 递归去判断这里是否合法, 最后一个是判断禁手的特殊
            board[row][col] = SPARE;                               // 再改回空位
        }
    }
    else if ((middle + side2) == 3 && jump2)
    {
        row -= (nearby2 + 1) * row_sign[sign];
        col -= (nearby2 + 1) * col_sign[sign];
        if (judge_in_bounds(row, col))
        {
            board[row][col] = BLACK;
            judge_going_4 += judge_legal_another(row, col, board); // 同上
            board[row][col] = SPARE;
        }
    }
    return judge_going_4;
}

// 判断是不是连着的4. 0表示没有四, 1表示一个冲四, 2表示扁担四, 4表示一个活四.
int judge_4(int side1, int side2, int jump1, int jump2, int middle)
{
    /*
        注明：0表示没有四, 1表示一个冲四, 2表示扁担四, 4表示一个活四.
        如果要求所有的, 我们取3的模, 得到的还是数字; 也有可能我们只关心活四和扁担四
    */

    if (middle == 4)
    {
        if (jump1 && jump2) // 两边有空
        {
            if (side1 && side2) // 两个潜在长连
                return 0;       // 啥也没有
            if (side1 || side2) // 一个潜在长连
                return 1;       // 一个冲四
                                // 没有
            return 4;           // 一个活四
        }
        else if (jump1 || jump2) // 一边有空, 另一边被白子堵住
        {
            if (side1 || side2) // 一个潜在长连
                return 0;       // 啥也没有
                                // 没有
            return 1;           // 一个冲四
        }
        // 两边没空
        return 0; // 啥也没有
    }
    if ((middle + side1 == 4) || (middle + side2 == 4)) // middle为4的情况已经排除
    {
        if (side1 == side2)
            return 2; // 扁担四
        return 1;     // 一个冲四
    }
    return 0; // 啥也没有
}
