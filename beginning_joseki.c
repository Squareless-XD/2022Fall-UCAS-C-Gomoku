#include "gomoku.h"

// 开局的一些定式选择, 返回1表示已落子, 返回0表示未落子
int beginning_joseki(int round, int player_turn)
{
    if (round == 1) // 如果正好是第一轮, 那么不用想了, 下中间就行了
        record_position(round, player_turn, 7, 7);

    else if (round == 2)
    {
        // 如果下在最中间
        if (inner_board_now[7][7] == BLACK)
            record_position(round, player_turn, 6, 7);

        // 在四个边上
        else if (row_now_global < 3 && col_now_global > 3 && col_now_global < 11)
            record_position(round, player_turn, row_now_global + 2, col_now_global);
        else if (row_now_global > 11 && col_now_global > 3 && col_now_global < 11)
            record_position(round, player_turn, row_now_global - 2, col_now_global);
        else if (col_now_global < 3 && row_now_global > 3 && row_now_global < 11)
            record_position(round, player_turn, row_now_global, col_now_global + 2);
        else if (col_now_global > 11 && row_now_global > 3 && row_now_global < 11)
            record_position(round, player_turn, row_now_global, col_now_global - 2);

        // 在四个角上
        else if (row_now_global <= 3 && col_now_global <= 3)
            record_position(round, player_turn, row_now_global + 2, col_now_global + 2);
        else if (row_now_global <= 3 && col_now_global >= 11)
            record_position(round, player_turn, row_now_global + 2, col_now_global - 2);
        else if (row_now_global >= 11 && col_now_global <= 3)
            record_position(round, player_turn, row_now_global - 2, col_now_global + 2);
        else if (row_now_global >= 11 && col_now_global >= 11)
            record_position(round, player_turn, row_now_global - 2, col_now_global - 2);

        else
            return 0;
    }
    else if (round == 3)
    {
        // 如果黑棋下在最中间
        if (inner_board_now[7][7] == BLACK)
        {
            // 在边上爬, 免得有人真这么写
            if (inner_board_now[0][0] == WHITE || inner_board_now[0][14] == WHITE || inner_board_now[14][0] == WHITE || inner_board_now[14][14] == WHITE)
                record_position(round, player_turn, 7, 8);

            // 横向或纵向相邻位置, 进入花月
            else if (inner_board_now[6][7] == WHITE || inner_board_now[7][8] == WHITE)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[7][6] == WHITE || inner_board_now[8][7] == WHITE)
                record_position(round, player_turn, 8, 6);

            // 斜向相邻位置, 进入浦月
            else if (inner_board_now[6][6] == WHITE || inner_board_now[8][8] == WHITE)
                record_position(round, player_turn, 8, 6);
            else if (inner_board_now[8][6] == WHITE || inner_board_now[6][8] == WHITE)
                record_position(round, player_turn, 6, 6);

            // 横向或纵向较远的位置
            else if (inner_board_now[5][7] == WHITE)
                record_position(round, player_turn, 8, 7);
            else if (inner_board_now[9][7] == WHITE)
                record_position(round, player_turn, 6, 7);
            else if (inner_board_now[7][5] == WHITE)
                record_position(round, player_turn, 7, 8);
            else if (inner_board_now[7][9] == WHITE)
                record_position(round, player_turn, 7, 6);

            // 斜对角较远的位置
            else if (inner_board_now[5][5] == WHITE || inner_board_now[9][9] == WHITE)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[5][9] == WHITE || inner_board_now[9][5] == WHITE)
                record_position(round, player_turn, 6, 6);
            else
                return 0;
        }
        else
            return 0;
    }
    else if (round == 4)
    {
        // 对于黑棋下在最中间的位置, 白棋肯定(作为机器自己)下在固定位置
        if (inner_board_now[7][7] == BLACK && inner_board_now[6][7] == WHITE)
        {
            // 花月 最强防
            if (inner_board_now[6][6] == BLACK)
                record_position(round, player_turn, 7, 6);
            else if (inner_board_now[6][8] == BLACK)
                record_position(round, player_turn, 7, 8);

            // 雨月 最强防
            else if (inner_board_now[7][6] == BLACK)
                record_position(round, player_turn, 7, 5);
            else if (inner_board_now[7][8] == BLACK)
                record_position(round, player_turn, 7, 9);

            // 丘月 最强防
            else if (inner_board_now[8][6] == BLACK)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[8][8] == BLACK)
                record_position(round, player_turn, 6, 6);

            // 松月 最强防
            else if (inner_board_now[8][7] == BLACK)
                record_position(round, player_turn, 9, 7);

            // 瑞星 最强防
            else if (inner_board_now[9][7] == BLACK)
                record_position(round, player_turn, 5, 6);

            // 寒星 最强防
            else if (inner_board_now[5][7] == BLACK)
                record_position(round, player_turn, 6, 6);

            // 晨星 最强防
            else if (inner_board_now[7][5] == BLACK)
                record_position(round, player_turn, 7, 6);
            else if (inner_board_now[7][9] == BLACK)
                record_position(round, player_turn, 7, 8);

            // 疏星 最强防
            else if (inner_board_now[5][5] == BLACK)
                record_position(round, player_turn, 6, 6);
            else if (inner_board_now[5][9] == BLACK)
                record_position(round, player_turn, 6, 8);

            else
                return 0;
        }
        else
            return 0;
    }
    else if (round == 5)
    {
        // 黑棋肯定下载中间位置. 如果白棋下在相邻位置, 第三手是机器下的, 一定是花月或浦月开局

        // 花月
        if (inner_board_now[7][7] == BLACK && inner_board_now[6][7] == WHITE && inner_board_now[6][8] == BLACK)
        {
            // 最强防
            if (inner_board_now[7][8] == WHITE)
                record_position(round, player_turn, 8, 9);
            // 次强防
            else if (inner_board_now[5][9] == WHITE)
                record_position(round, player_turn, 8, 6);
            else if (inner_board_now[8][6] == WHITE)
                record_position(round, player_turn, 5, 9);
            // 三防
            else if (inner_board_now[5][6] == WHITE)
                record_position(round, player_turn, 7, 8);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[7][8] == WHITE && inner_board_now[6][8] == BLACK)
        {
            if (inner_board_now[6][7] == WHITE)
                record_position(round, player_turn, 8, 9);
            else if (inner_board_now[5][9] == WHITE)
                record_position(round, player_turn, 8, 6);
            else if (inner_board_now[8][6] == WHITE)
                record_position(round, player_turn, 5, 9);
            else if (inner_board_now[8][9] == WHITE)
                record_position(round, player_turn, 6, 7);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[7][6] == WHITE && inner_board_now[8][6] == BLACK)
        {
            if (inner_board_now[8][7] == WHITE)
                record_position(round, player_turn, 9, 8);
            else if (inner_board_now[9][5] == WHITE)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[6][8] == WHITE)
                record_position(round, player_turn, 9, 5);
            else if (inner_board_now[6][5] == WHITE)
                record_position(round, player_turn, 8, 7);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[8][7] == WHITE && inner_board_now[8][6] == BLACK)
        {
            if (inner_board_now[7][6] == WHITE)
                record_position(round, player_turn, 9, 8);
            else if (inner_board_now[9][5] == WHITE)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[6][8] == WHITE)
                record_position(round, player_turn, 9, 5);
            else if (inner_board_now[9][8] == WHITE)
                record_position(round, player_turn, 7, 6);
            else
                return 0;
        }

        // 浦月
        else if (inner_board_now[7][7] == BLACK && inner_board_now[6][6] == WHITE && inner_board_now[8][6] == BLACK)
        {
            // 最强防
            if (inner_board_now[6][8] == WHITE)
                record_position(round, player_turn, 8, 5);
            // 同样位置的三个强防和三个弱防
            else if (inner_board_now[9][5] == WHITE || inner_board_now[8][7] == WHITE || inner_board_now[9][7] == WHITE || inner_board_now[9][6] == WHITE || inner_board_now[7][5] == WHITE || inner_board_now[7][6] == WHITE)
                record_position(round, player_turn, 6, 8);
            // 弱防
            else if (inner_board_now[8][5] == WHITE)
                record_position(round, player_turn, 9, 5);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[8][8] == WHITE && inner_board_now[8][6] == BLACK)
        {
            if (inner_board_now[6][8] == WHITE)
                record_position(round, player_turn, 9, 6);
            else if (inner_board_now[9][5] == WHITE || inner_board_now[8][7] == WHITE || inner_board_now[9][7] == WHITE || inner_board_now[8][5] == WHITE || inner_board_now[7][5] == WHITE || inner_board_now[7][6] == WHITE)
                record_position(round, player_turn, 6, 8);
            else if (inner_board_now[9][6] == WHITE)
                record_position(round, player_turn, 9, 5);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[8][6] == WHITE && inner_board_now[6][6] == BLACK)
        {
            if (inner_board_now[8][8] == WHITE)
                record_position(round, player_turn, 6, 5);
            else if (inner_board_now[5][5] == WHITE || inner_board_now[6][7] == WHITE || inner_board_now[5][7] == WHITE || inner_board_now[5][6] == WHITE || inner_board_now[7][5] == WHITE || inner_board_now[7][6] == WHITE)
                record_position(round, player_turn, 8, 8);
            else if (inner_board_now[6][5] == WHITE)
                record_position(round, player_turn, 5, 5);
            else
                return 0;
        }
        else if (inner_board_now[7][7] == BLACK && inner_board_now[6][8] == WHITE && inner_board_now[6][6] == BLACK)
        {
            if (inner_board_now[8][8] == WHITE)
                record_position(round, player_turn, 5, 6);
            else if (inner_board_now[5][5] == WHITE || inner_board_now[6][7] == WHITE || inner_board_now[5][7] == WHITE || inner_board_now[6][5] == WHITE || inner_board_now[7][5] == WHITE || inner_board_now[7][6] == WHITE)
                record_position(round, player_turn, 8, 8);
            else if (inner_board_now[5][6] == WHITE)
                record_position(round, player_turn, 5, 5);
            else
                return 0;
        }
        else
            return 0;
    }
    else
        return 0;

    // 返回1表示继续
    return 1;
}
