#include "gomoku.h"

// This program use UTF-8 encoding

int inner_board_now[BOARD_SIZE][BOARD_SIZE]; // 当前的棋盘的格局

int col_now_global;                     // 刚才下的棋子的行位置
int row_now_global;                     // 刚才下的棋子的列位置
int row_black_global;                   // 刚才下的黑棋的行位置
int col_black_global;                   // 刚才下的黑棋的列位置
int row_white_global;                   // 刚才下的白棋的行位置
int col_white_global;                   // 刚才下的白棋的列位置
int row_black_global_back[MAX_RETRACT]; // 再之前下的黑棋的行位置
int col_black_global_back[MAX_RETRACT]; // 再之前下的黑棋的列位置
int row_white_global_back[MAX_RETRACT]; // 再之前下的白棋的行位置
int col_white_global_back[MAX_RETRACT]; // 再之前下的白棋的列位置
int retract_times_left_black;           // 剩余悔棋次数
int retract_times_left_white;           // 剩余悔棋次数

char const play1_pic[] = "●";         // 之前下的黑棋子
char const play1_current_pic[] = "▲"; // 刚才下的黑棋子
char const play2_pic[] = "◎";         // 之前下的白棋子
char const play2_current_pic[] = "△"; // 刚才下的白棋子

struct POSITION game_record[MAX_RECORD_SIZE]; // 棋谱记录
int first_layer_max_used_width = 0;           // 第一层用到的最靠后的结点

int tuple_set_hori[BOARD_SIZE][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 横向
int tuple_set_topr[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 左下-右上
int tuple_set_vert[TUPLE_NUMB][BOARD_SIZE][TUPLE_SIZE]; // 打分用的六元组: 纵向
int tuple_set_topl[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 左上-右下

int point_hori[BOARD_SIZE][TUPLE_NUMB]; // horizontal 打分六元组: 横向
int point_topr[TUPLE_NUMB][TUPLE_NUMB]; // top_right  打分六元组: 左下-右上
int point_vert[TUPLE_NUMB][BOARD_SIZE]; // vertical   打分六元组: 纵向
int point_topl[TUPLE_NUMB][TUPLE_NUMB]; // top_left   打分六元组: 左上-右下

// 方向棋盘变动时, row和col加减的数 (方向矢量), 就是一个参数数组
int const row_sign[8] = {1, 1, 0, -1, -1, -1, 0, 1};
int const col_sign[8] = {0, 1, 1, 1, 0, -1, -1, -1};

struct TNODE *root = NULL; // 全局通用的根节点, 因为每次只有一个树

int main(int argc, char *argv[])
{
    int board_copy[BOARD_SIZE][BOARD_SIZE]; // 此数组用于记录棋盘副本, 供编辑使用
    int player_turn;                        // 该谁下了, 1代表黑棋, 2代表白棋
    int game_over;                          // 游戏是否已经结束, 1代表已结束
    int row;                                // 输入到棋盘的行, 从0到14
    int col;                                // 输入到棋盘的列, 从0到14
    int place_error;                        // 用于返回输入的棋子能不能下, 0是可以下, 1是不能下, 2是终止游戏, 3是重启游戏
    int legal;                              // 黑棋是否触犯禁手, 0是违法, 1是不违法
    int win;                                // 谁赢了, 0代表未结束, 1代表黑棋, 2代表白棋, 3代表平局
    int round;                              // 下到第几轮了
    int i;                                  // 用在for循环中的字母i
    int game_mode;                          // 游戏模式, 1代表人人对战, 2代表人机对战
    int first_player;                       // 先手与否, 1代表人是先手, 2代表人是后手

restart_position:

    // 开始初始化

    round = 1;           // 一开始是第一轮
    player_turn = BLACK; // 该黑棋落子
    game_over = 0;       // 游戏未结束
    win = 0;             // 也没有人取得胜利
    game_mode = 0;       // 初始化游戏模式
    first_player = 0;    // 初始化玩家是否先手

    tree_free(root);                     // 释放树存留的结点
    mytree_set_root();                   // root初始化
    clear_variables();                   // 清零各个全局变量
    init_record_borard(inner_board_now); // 初始化游戏用棋盘

    // 清屏并显示信息

    system("clear"); // 清屏
    printf("%s%s%s%s%s%s%s%s%s",
           "Welcom to Gomoku by Junxiong Ji!\n\n",
           "Here you can choose two mode: PVP or PVE,\n",
           "and you can choose to go first or go next.\n",
           "Type down \"0\", \"quit\", \"end\" or \"stop\" to quit the game,\n",
           "\"restart\" to restart the game,\n",
           "\"regret\", \"retract\" or \"back\" to retract last stone\n",
           "(and you can choose which side is regretting).\n",
           "Almost all kinds of input are allowed,\n",
           "like \"A01\", \"A1\", \"1A\", \"01A\", and \'a\' in lower case.\n");

    // 开始程序

    print_board(inner_board_now); // 打印棋盘

    do
    {
        game_mode = mode_choose(MODE); // 选取模式
    } while (game_mode != GAME_STOP && game_mode != PLAYER_PLAYER && game_mode != PLAYER_COMPUTER);
    if (game_mode == GAME_STOP) // 输入是EOF或者0, 结束游戏
        return 0;
    if (game_mode == PLAYER_COMPUTER) // 选取人机对战, 继续选取先后手
    {
        first_player = mode_choose(FIRST); // 选取先手
        if (first_player == GAME_STOP)     // 输入是EOF或者0, 结束游戏
            return 0;
    }

    // 在游戏未结束时执行下棋的循环
    while (!game_over)
    {
        if (game_mode == PLAYER_PLAYER || player_turn == first_player) // 轮到了玩家下棋
        {
            place_error = place_stone(player_turn, round, game_mode); // 检查能不能落子

            if (place_error == INPUT_WRONG) // 落子位置不对或者有子, 需要重新落子
                continue;
            else if (place_error == END_GAME) // 终止游戏
            {
                game_over = GAME_OVER;
                break;
            }
            else if (place_error == RESTART_GAME) // 重启游戏
                goto restart_position;
            else if (place_error == RETRACT_ONCE) // 悔棋一步
            {
                round -= 1;
                player_turn = BLACK + WHITE - player_turn;
                print_board(inner_board_now);
                continue;
            }
            else if (place_error == RETRACT_TWICE) // 悔棋两步
            {
                round -= 2;
                print_board(inner_board_now);
                continue;
            }
        }
        else // 轮到了机器下棋
        {
            computer_placing(player_turn, round); // 调用机器落子的函数
            printf("\n机器选择的落子位置:%c%d\n", 'A' + col_now_global, BOARD_SIZE - row_now_global);
        }

        copy_inner_board_to_copy(board_copy); // 拷贝一个棋盘的副本

        if (player_turn == BLACK) // 黑棋下才判断禁手
            legal = judge_legal(row_now_global, col_now_global, board_copy);

        print_board(inner_board_now);         // 显示棋盘
        copy_inner_board_to_copy(board_copy); // 拷贝一个棋盘的副本

        if (!legal) // 触犯禁手
        {
            game_over = GAME_OVER;
            win = WHITE;
            printf("黑棋触犯禁手了，白棋获胜。\n");
            break;
        }
        else if (judge_winning(player_turn, row_now_global, col_now_global, board_copy)) // 未触犯禁手且胜利
        {
            game_over = GAME_OVER;
            win = player_turn;
            if (player_turn == BLACK)
                printf("黑棋获胜。\n");
            else
                printf("白棋获胜。\n");
            break;
        }
        else if (round >= BOARD_SIZE * BOARD_SIZE)
        {
            game_over = GAME_OVER;
            win = _EQUAL;
            printf("棋盘下满，双方平局！\n");
            break;
        }

        ++round;                                   // 下一轮
        player_turn = BLACK + WHITE - player_turn; // 切换下棋方
    }
    // 重新开始循环

    // 结束程序并选择是否需要记录棋谱

#ifndef _NORECORD
    // 记录棋谱
    final_recording(game_mode, first_player, round, win);
#endif

    return 0;
}
