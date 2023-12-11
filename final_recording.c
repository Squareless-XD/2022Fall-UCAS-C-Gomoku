#include "gomoku.h"

// 最终进行的棋谱记录
int final_recording(int game_mode, int first_player, int round, int winner)
{
    char curtime[30];
    char file_name[50];
    struct tm *pcurtime;
    FILE *fp;
    int i;
    char winner_text[15];
    int time;

    // 记录
    pcurtime = get_time_tm();
    sprintf(curtime, "%04d-%02d-%02d_%02d_%02d_%02d",
            (pcurtime->tm_year + 1900) % 10000, pcurtime->tm_mon % 100 + 1,
            pcurtime->tm_mday % 100, pcurtime->tm_hour % 100, pcurtime->tm_min % 100, pcurtime->tm_sec % 100);

    sprintf(file_name, "./game_records/%s.txt", curtime);

    fp = fopen(file_name, "w");
    if (fp == NULL)
        return 1;
    switch (winner)
    {
    case 0:
        sprintf(winner_text, "unfinished");
        break;
    case 1:
        sprintf(winner_text, "black");
        break;
    case 2:
        sprintf(winner_text, "white");
        break;
    case 3:
        sprintf(winner_text, "draw");
        break;
    default:
        sprintf(winner_text, "unknown");
        break;
    }

    fprintf(fp, "file name: %s\n", curtime);

    if (game_mode == PLAYER_PLAYER)
        fprintf(fp, "gamemode: PVP\n");
    else if (game_mode == PLAYER_COMPUTER)
        fprintf(fp, "gamemode: PVE       player goes %s\n", (first_player == BLACK) ? "first" : "next");

    fprintf(fp, "round: %-3d          winner: %s\n", round, winner_text);
    fprintf(fp, "search depth: %-3d   width: %-2d    max used width of the first floor: %d\n", TREE_DEPTH, TREE_WIDTH, first_layer_max_used_width);
    fprintf(fp, "\n");

    if (round < 100)
        for (i = 1; i <= round; i++)
            fprintf(fp, "%sstep %2d: %c%-2d%s",
                    (i % 2) ? "" : "\t\t",
                    i,
                    'A' + game_record[i].col,
                    BOARD_SIZE - game_record[i].row,
                    (i % 2) ? "" : "\n");
    else
        for (i = 1; i <= round; i++)
            fprintf(fp, "%sstep %3d: %c%-2d%s",
                    (i % 2) ? "" : "\t\t",
                    i,
                    'A' + game_record[i].col,
                    BOARD_SIZE - game_record[i].row,
                    (i % 2) ? "" : "\n");

    if (round % 2)
        fprintf(fp, "\n");
    fprintf(fp, "\nfinal chessboard:\n");
    fprint_board(inner_board_now, fp);

    fclose(fp);

    return 0;
}

// 得到struct tm *格式的时间
struct tm *get_time_tm()
{
    time_t time_string;
    time(&time_string);
    return localtime(&time_string);
}

// 打印棋盘到文件
void fprint_board(int board[BOARD_SIZE][BOARD_SIZE], FILE *fp)
{
    fprintf(fp, "\n  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        fprintf(fp, " %c", 'A' + i);
    fprintf(fp, "\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        fprintf(fp, "%2d ", BOARD_SIZE - i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == BLACK) // 是黑棋
            {
                if (i == row_black_global && j == col_black_global)
                    fprintf(fp, "▲");
                else
                    fprintf(fp, "●");
            }
            else if (board[i][j] == WHITE) // 是白棋
            {
                if (i == row_white_global && j == col_white_global)
                    fprintf(fp, "△");
                else
                    fprintf(fp, "◎");
            }
            else if (i == 0) // 最上方一行的空白
            {
                if (j == 0)
                    fprintf(fp, "┌");
                else if (j == BOARD_SIZE - 1)
                    fprintf(fp, "┐");
                else
                    fprintf(fp, "┬");
            }
            else if (i == BOARD_SIZE - 1) // 最下方一行的空白
            {
                if (j == 0)
                    fprintf(fp, "└");
                else if (j == BOARD_SIZE - 1)
                    fprintf(fp, "┘");
                else
                    fprintf(fp, "┴");
            }
            else // 其余行的空白
            {
                if (j == 0)
                    fprintf(fp, "├");
                else if (j == BOARD_SIZE - 1)
                    fprintf(fp, "┤");
                else
                    fprintf(fp, "┼");
            }

            if (j != BOARD_SIZE - 1)
                fprintf(fp, "─");
        }
        fprintf(fp, "% 2d\n", BOARD_SIZE - i);
    }

    fprintf(fp, "  ");
    for (int i = 0; i < BOARD_SIZE; i++)
        fprintf(fp, " %c", 'A' + i);
    fprintf(fp, "\n");
}
