#include "gomoku.h"

// 打分
ll evaluation(int player_turn, int row, int col, int whether_judge_legal)
{
    int i, j, k;
    int pattern;
    ll point = 0;
    place_stone_on_copy(row, col, player_turn);

    // 几种需要返回分值的情况

    if (judge_winning(player_turn, row, col, chessboard_copy) == GAME_OVER) // 下到这一步就已经赢了. 但建议继续考虑打分, 因为本来打分不齐全, 这样能做出一个更好的排序
        point = (player_turn == BLACK) ? POINT_WIN : -POINT_WIN;
    else if (whether_judge_legal == 1 && player_turn == BLACK && judge_legal(row, col, chessboard_copy) == ILLEGAL_PLACING) // 如果黑棋走了禁手! 前提是输入的时候要求判断
    {
        point = -POINT_BAN;
        place_stone_on_copy(row, col, SPARE);
        return point;
    }
    // 打分函数. 这里的对point的操作用的都是赋值运算符, 因为要在之前对胜利情况考虑的基础之上
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < TUPLE_NUMB; j++)
        {
            point += point_copy_hori[i][j];
            point += point_copy_vert[j][i];
        }
    for (i = 0; i < TUPLE_NUMB; i++)
        for (j = 0; j < TUPLE_NUMB; j++)
        {
            point += point_copy_topr[i][j];
            point += point_copy_topl[i][j];
        }
    place_stone_on_copy(row, col, SPARE);

    return point;
}

// 查表得到元组对应的是多少分
int get_grade_for_tuple(int sequence)
{
    int point = 0;

    // 下面的case列举, 使用的是0开头的8进制. 对应我们之前对pattern的左移三位的操作

    switch (sequence)        // 按照case枚举每一种情况, 一共只有012, 并且赋分. 需要注意的是, 黑棋和白棋的给分是不一样的
    {
        case 0000000:                   point =  3;     break;          // 同样多个数的棋子的结尾
        case 0000001:  case 0100000:    point =  6;     break;
        case 0000010:  case 0010000:    point =  6;     break;
        case 0000100:  case 0001000:    point =  6;     break;          // 同样多个数的棋子的结尾

        case 0000011:  case 0110000:    point =  20;    break;
        case 0000101:  case 0101000:    point =  20;    break;
        case 0001001:  case 0100100:    point =  8;     break;
        case 0010001:  case 0100010:    point =  8;     break;
        case 0100001:                   point = -30;    break;     // 分类
        case 0000110:  case 0011000:    point =  100;   break;
        case 0001010:  case 0010100:    point =  100;   break;
        case 0010010:                   point =  20;    break;
        case 0001100:                   point =  200;   break;          // 同样多个数的棋子的结尾

        case 0000111:  case 0111000:    point =  700;   break;
        case 0001011:  case 0110100:    point =  550;   break;
        case 0010011:  case 0110010:    point =  400;   break;
        case 0100011:  case 0110001:    point = -150;   break;    // 分类
        case 0001101:  case 0101100:    point =  450;   break;
        case 0010101:  case 0101010:    point =  400;   break;
        case 0100101:  case 0101001:    point = -150;   break;    // 分类
        case 0011001:  case 0100110:    point =  350;   break;
        case 0001110:  case 0011100:    point =  6000;  break;
        case 0010110:  case 0011010:    point =  4000;  break;          // 同样多个数的棋子的结尾

        case 0211112:                   point = -100;   break;
        case 0211102:  case 0201112:    point = -150;   break;
        case 0211012:  case 0210112:    point = -150;   break;
        case 0210102:  case 0201012:    point = -180;   break;
        case 0210012:                   point = -130;   break;
        case 0201102:                   point = -100;   break;

        case 0211120:                   point = -200;   break;
        case 0211020:  case 0201120:    point = -120;   break;
        case 0210120:                   point = -150;   break;
        case 0210020:                   point = -25;    break;
        case 0021112:                   point = -200;   break;
        case 0021102:  case 0020112:    point = -120;   break;
        case 0021012:                   point = -150;   break;
        case 0021002:                   point = -25;    break;

        case 0211121:                   point = -200;   break;
        case 0211021:  case 0201121:    point = -120;   break;
        case 0210121:                   point = -150;   break;
        case 0210021:                   point = -25;    break;
        case 0121112:                   point = -200;   break;
        case 0121102:  case 0120112:    point = -120;   break;
        case 0121012:                   point = -150;   break;
        case 0121002:                   point = -25;    break;

        case 0001111:  case 0111100:    point =  8000;  break;
        case 0010111:  case 0111010:    point =  6000;  break;
        case 0100111:  case 0111001:    point = -500;   break;    // 分类
        case 0011011:  case 0110110:    point =  6000;  break;
        case 0101011:  case 0110101:    point = -1000;  break;    // 分类
        case 0110011:                   point = -1000;  break;    // 分类
        case 0011101:  case 0101110:    point =  6000;  break;
        case 0101101:                   point = -3000;  break;    // 分类
        case 0011110:                   point =  50000; break;

        case 0000002:  case 0200000:    point = -2;     break;
        case 0000020:  case 0020000:    point = -3;     break;
        case 0000200:  case 0002000:    point = -3;     break;          // 同样多个数的棋子的结尾

        case 0000022:  case 0220000:    point = -10;    break;
        case 0000202:  case 0202000:    point = -10;    break;
        case 0002002:  case 0200200:    point = -6;     break;
        case 0020002:  case 0200020:    point = -6;     break;
        case 0200002:                   point = -5;     break;     // 分类
        case 0000220:  case 0022000:    point = -100;   break;
        case 0002020:  case 0020200:    point = -100;   break;
        case 0020020:                   point = -20;    break;
        case 0002200:                   point = -200;   break;          // 同样多个数的棋子的结尾

        case 0122221:                   point =  100;   break;
        case 0122201:  case 0102221:    point =  150;   break;
        case 0122021:  case 0120221:    point =  150;   break;
        case 0120201:  case 0102021:    point =  180;   break;
        case 0120021:                   point =  130;   break;
        case 0102201:                   point =  100;   break;

        case 0122210:                   point =  200;   break;
        case 0122010:  case 0102210:    point =  120;   break;
        case 0120210:                   point =  150;   break;
        case 0120010:                   point =  25;    break;
        case 0012221:                   point =  200;   break;
        case 0012201:  case 0010221:    point =  120;   break;
        case 0012021:                   point =  150;   break;
        case 0012001:                   point =  25;    break;

        case 0122212:                   point =  200;   break;
        case 0122012:  case 0102212:    point =  120;   break;
        case 0120212:                   point =  150;   break;
        case 0120012:                   point =  25;    break;
        case 0212221:                   point =  200;   break;
        case 0212201:  case 0210221:    point =  120;   break;
        case 0212021:                   point =  150;   break;
        case 0212001:                   point =  25;    break;

        case 0000222:  case 0222000:    point = -700;   break;
        case 0002022:  case 0220200:    point = -550;   break;
        case 0020022:  case 0220020:    point = -400;   break;
        case 0200022:  case 0220002:    point = -50;    break;    // 分类
        case 0002202:  case 0202200:    point = -450;   break;
        case 0020202:  case 0202020:    point = -400;   break;
        case 0200202:  case 0202002:    point = -130;   break;    // 分类
        case 0022002:  case 0200220:    point = -350;   break;
        case 0002220:  case 0022200:    point = -6000;  break;
        case 0020220:  case 0022020:    point = -4000;  break;          // 同样多个数的棋子的结尾

        case 0002222:  case 0222200:    point = -8000;  break;
        case 0020222:  case 0222020:    point = -6000;  break;
        case 0200222:  case 0222002:    point = -4200;  break;    // 分类
        case 0022022:  case 0220220:    point = -5000;  break;
        case 0202022:  case 0220202:    point = -800;   break;    // 分类
        case 0220022:                   point = -400;   break;    // 分类
        case 0022202:  case 0202220:    point = -6000;  break;
        case 0202202:                   point = -500;   break;    // 分类
        case 0022220:                   point = -50000; break;
    }
    // if (point < 0)
    //     return point * 9 / 10;
    return point;
}
