#include "gomoku.h"

// 初始化一个空棋盘格局
void init_record_borard(int empty_board[BOARD_SIZE][BOARD_SIZE])
{
    int i, j;
    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            empty_board[i][j] = 0; // 将当前棋盘清零
}

// 复制棋盘到board_copy上
void copy_inner_board_to_copy(int board_copy[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
        for (int j = 0; j < BOARD_SIZE; j++)
            board_copy[i][j] = inner_board_now[i][j]; // 复制当前棋盘
}

#ifdef _WIDE

// 打印棋盘, 字符为较宽的2:2
void print_board(int board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf(" %c", 'A' + i);
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%2d ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == BLACK) // 是黑棋
            {
                if (i == row_black_global && j == col_black_global)
                    printf("▲");
                else
                    printf("●");
            }
            else if (board[i][j] == WHITE) // 是白棋
            {
                if (i == row_white_global && j == col_white_global)
                    printf("△");
                else
                    printf("◎");
            }
            else if (i == 0) // 最上方一行的空白
            {
                if (j == 0)
                    printf("┌");
                else if (j == BOARD_SIZE - 1)
                    printf("┐");
                else
                    printf("┬");
            }
            else if (i == BOARD_SIZE - 1) // 最下方一行的空白
            {
                if (j == 0)
                    printf("└");
                else if (j == BOARD_SIZE - 1)
                    printf("┘");
                else
                    printf("┴");
            }
            else // 其余行的空白
            {
                if (j == 0)
                    printf("├");
                else if (j == BOARD_SIZE - 1)
                    printf("┤");
                else
                    printf("┼");
            }
        }
        printf("% 2d\n", BOARD_SIZE - i);
    }

    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf(" %c", 'A' + i);
    printf("\n");
}

#else

// 打印棋盘, 字符为正常的1:2
void print_board(int board[BOARD_SIZE][BOARD_SIZE])
{
    printf("\n  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf(" %c", 'A' + i);
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%2d ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == BLACK) // 是黑棋
            {
                if (i == row_black_global && j == col_black_global)
                    printf("▲");
                else
                    printf("●");
            }
            else if (board[i][j] == WHITE) // 是白棋
            {
                if (i == row_white_global && j == col_white_global)
                    printf("△");
                else
                    printf("◎");
            }
            else if (i == 0) // 最上方一行的空白
            {
                if (j == 0)
                    printf("┌");
                else if (j == BOARD_SIZE - 1)
                    printf("┐");
                else
                    printf("┬");
            }
            else if (i == BOARD_SIZE - 1) // 最下方一行的空白
            {
                if (j == 0)
                    printf("└");
                else if (j == BOARD_SIZE - 1)
                    printf("┘");
                else
                    printf("┴");
            }
            else // 其余行的空白
            {
                if (j == 0)
                    printf("├");
                else if (j == BOARD_SIZE - 1)
                    printf("┤");
                else
                    printf("┼");
            }

            if (j != BOARD_SIZE - 1)
                printf("─");
        }
        printf("% 2d\n", BOARD_SIZE - i);
    }

    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        printf(" %c", 'A' + i);
    printf("\n");
}

#endif
