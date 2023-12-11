//
// Attention: use UTF-8 to open this file !!!
// Attention: use UTF-8 to open this file !!!
// Attention: use UTF-8 to open this file !!!
//
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
// &                                                                                                   &
// &  %%%%%%%%%%%       %%            %%           %%%%      %%%%%%%%%%%      %%%%%%        %%%%%%     &
// &  %%              %%%%          %%%%         %%    %%    %%             %%      %%    %%      %%   &
// &  %%                %%            %%         %%    %%    %%            %%        %%  %%        %%  &
// &  %%                %%            %%          %% %%%     %%            %%        %%   %%      %%%  &
// &   %%%%%%%%         %%            %%          %%%         %%%%%%%%%    %%        %%    %%%%%%% %%  &
// &           %%       %%            %%        %%   %%   %           %%   %%        %%            %%  &
// &            %%      %%            %%       %%    %%% %%            %%  %%        %%           %%   &
// &  %%       %%       %%            %%        %%     %%%   %%       %%    %%      %%    %%     %%    &
// &    %%%%%%%      %%%%%%%%      %%%%%%%%       %%%%%  %%    %%%%%%%        %%%%%%        %%%%%%     &
// &                                                                                                   &
// &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
// Clang project of the University of Chinese Academy of Sciences
// By Junxiong Ji
// Jan. 1st, 2023
//
// Using global evaluation, minimax search and alpha beta pruning, with editable search depth and width.
// Some stable beginning pattern were used to promote computer's performance.
// Able to choose PVP and PVE two mode.
// With stable algorithm without random value, computer v.s. computer mode is not provided.
// After the game, an record txt will be automatically generated.
//
// Attention: use UTF-8 to open this file !!!
//

#ifndef _GOMOKU_H

#define _GOMOKU_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

////////////////////////////////////////////////////
//                                                //
// some editable parameters are listed below      //
//                                                //
#define TREE_DEPTH 10 // minimax递归向下调用的层数
#define TREE_WIDTH 11 // minimax决策树的宽度
#define MAX_RETRACT 3 // 可以悔棋的次数
//                                                //
// some editable parameters are listed above      //
//                                                //
////////////////////////////////////////////////////

#define CHARSIZE 3          // 棋子符号的字节数. 并没有用到次变量, 但是需要注意的是, 这是UTF-8编码下的结果
#define MAX_RECORD_SIZE 250 // 棋谱的最大记录个数
#define BOARD_SIZE 15       // 棋盘的大小
#define PLAYER 2            // 玩家数
#define DIMENSION 2         // 维度数
#define MAX_INPUT 15        // 最大的输入长度
#define MAXLEN 10           // 输入字符串最长的长度

#define LEGAL_PLACING 1   // 落子不属于禁手
#define ILLEGAL_PLACING 0 // 落子属于禁手
#define HALF_DIRECTION 4  // 八个方向的一半: 四个方向
#define DIRECTION 8       // 八个方向
#define IN_BOUND 1        // 在棋盘内
#define BEYOND_BOUND 0    // 不在棋盘内
#define GAME_OVER 1       // 落子使玩家取得胜利
#define GAME_CONTINUE 0   // 落子暂时没有使玩家取得胜利

#define TUPLE_SIZE 6  // 打分用的五元组的长度
#define TUPLE_NUMB 10 // 打分用的五元组在限制方向时的个数

#define ll long long // 长整型

#define ADD_TO_LIST_HEAD 1 // 加到链表的前面
#define ADD_TO_LIST_TAIL 0 // 加到链表的后面

#define POINT_WIN 1000000   // 嬴棋的打分 (绝对值)
#define POINT_BAN 10000000  // 禁手的打分 (绝对值)
#define INFINITY 1000000000 // 认为这个数字是无穷, 够大了 int最大才2e9, 这个都1e9了

// 以下的 enum 仅用作对定义的解释, 即作为宏的 "等价" 写法, 并不会实际用到这些枚举类型

enum stone_color // 表示棋子的颜色
{
    SPARE,     // 空闲, 用0表示
    BLACK,     // 之前下的黑棋子, 用1表示
    WHITE,     // 之前下的白棋子, 用2表示
    BLACK_NOW, // 刚才下的黑棋子, 用3表示
    WHITE_NOW  // 刚才下的白棋子, 用4表示
};
enum final_winner // 最终的胜者是谁. 用于对比赛结果的判断, 存储在结果文档中
{
    _UNFINISHED, // 未结束
    _BLACK,      // 黑棋胜利
    _WHITE,      // 白棋胜利
    _EQUAL       // 平局
};
enum placing_type // 记录玩家输入的类型
{
    POSITION_AVAILABLE, // 可以下在这里
    INPUT_WRONG,        // 棋子的落子输入错误
    END_GAME,           // 输入 end, quit, stop 结束游戏
    RESTART_GAME,       // 输入 restart 重开游戏
    RETRACT_ONCE,       // 悔棋一步
    RETRACT_TWICE       // 悔棋两步
};
enum gamemode_type // 记录游戏模式输入的结果
{
    GAME_STOP,       // 终止游戏
    PLAYER_PLAYER,   // 人人对战模式
    PLAYER_COMPUTER, // 人机对战模式
    INVALID_INPUT,   // 输入无效
};
enum input_mode_choose // 作为mode_choose的参数, 仅改变棋盘上打印的输出
{
    MODE,    // 选择游戏模式是人人还是人机
    FIRST,   // 选择玩家是先手还是后手
    RETRACT, // 选择哪个玩家悔棋
};

// 以下为一些实际使用到的结构体

struct ELEMENT // 一个结点中的性质
{
    ll point;
    ll point_diff;
    int row;
    int col;
};

struct POSITION // 一个结点中的性质
{
    int row;
    int col;
};

struct TNODE // 树结点
{
    ll point;
    int row;
    int col;
    struct TNODE *child;
    struct TNODE *sibling;
};

struct LISTNODE // 单向链表结点
{
    int row;
    int col;
    struct LISTNODE *next;
};

/*
    函数声明
*/

// 清零六元组
void clear_variables(void);

// 选取游戏模式
int mode_choose(int mode_or_first);

// 获取stdin中的一行输入, 并且将stdin后续的字符清除, 直至'\n'或EOF. 返回字符串的长度, 若读到了EOF, 返回-1
int get_full_line(char s[], int lim);

// 负责落子的函数
int place_stone(int player_turn, int round, int game_mode);

// 悔棋
int retract_stone(int player_turn, int round, int game_mode);

// 撤回一个黑子, 操作和落子时的操作类似
void retract_black(void);

// 撤回一个白子, 操作和落子时的操作类似
void retract_white(void);

// 在真正的棋盘上落子或者取消落子
void place_stone_player(int row, int col, int stone);

// 判断是否越界的函数
int judge_in_bounds(int row, int col);

// 判断是否存在活3的函数
int judge_3(int nearby1, int nearby2, int side1, int side2, int jump1, int jump2, int row, int col, int sign, int board[BOARD_SIZE][BOARD_SIZE]);

// 判断是不是连着的4. 0表示没有四, 1表示一个冲四, 2表示扁担四, 4表示一个活四.
int judge_4(int side1, int side2, int jump1, int jump2, int middle);

// 判断是不是禁手
int judge_legal(int row_place, int col_place, int board[BOARD_SIZE][BOARD_SIZE]);

// 另一个判断是不是禁手, 供活三判断递归中使用的函数, 仅用于计算活四
int judge_legal_another(int row_place, int col_place, int board[BOARD_SIZE][BOARD_SIZE]);

// 判断玩家是否已经取胜
int judge_winning(int player_turn, int row_place, int col_place, int board[BOARD_SIZE][BOARD_SIZE]);

// 初始化一个空棋盘格局
void init_record_borard(int empty_board[BOARD_SIZE][BOARD_SIZE]);

// 复制棋盘到board_copy上
void copy_inner_board_to_copy(int board_copy[BOARD_SIZE][BOARD_SIZE]);

// 打印棋盘
void print_board(int board[BOARD_SIZE][BOARD_SIZE]);

// 求最大值
ll max(ll a, ll b);

// 求最小值
ll min(ll a, ll b);

// 给树建立一个新的孩子, 自动选择是在child处还是在child的sibling处建立
int mytree_set_child(struct TNODE *node, int row, int col, ll point);

// 给树的根节点刷新
int mytree_set_root();

// 新建一个TNODE
struct TNODE *talloc(void);

// 新建一个ELEMENT
struct ELEMENT *ealloc(void);

// 将树的结点全部释放
void tree_free(struct TNODE *head);

// 实现机器落子的函数. 需要: 该谁下player_turn, 第几轮round, 棋盘
int computer_placing(int playerturn, int round);

// 开局的一些定式选择, 返回1表示已落子, 返回0表示未落子
int beginning_joseki(int round, int player_turn);

// minimax法, 对决策树做alpha-beta剪枝
ll minimax(struct LISTNODE *old_list_head, struct TNODE *head, int depth, int alpha, int beta, int round, int player_turn);

// 建立第一层新的可能性, 仅限第一层
struct LISTNODE *create_first_floor(struct TNODE *thead, int player_turn, int round, int depth);

// 建立一层新的可能性
struct LISTNODE *create_new_floor(struct LISTNODE *old_list_head, struct TNODE *tree_head, int row, int col, int player_turn, int round, int depth);

// 在copy上落子或者取消落子
void place_stone_on_copy(int row, int col, int stone);

// 打分
ll evaluation(int player_turn, int row, int col, int whether_judge_legal);

// 查表得到元组对应的是多少分
int get_grade_for_tuple(int sequence);

// 给链表新增元素
int list_set_new(int row, int col, int add_to_head, struct LISTNODE **plist_head, struct LISTNODE **plist_tail);

// 给链表加一个开头
void list_set_head(int row, int col, struct LISTNODE **plist_head, struct LISTNODE **plist_tail);

// 新建一个LISTNODE
struct LISTNODE *lalloc(void);

// 将链表的结点全部释放
void list_free(struct LISTNODE *list_head);

// 给链表开头加一个结点
void list_add_head(int row, int col, struct LISTNODE **plist_head);

// 给链表结尾加一个结点
void list_add_tail(int row, int col, struct LISTNODE **plist_tail);

// 在链表中搜索一个结点
struct LISTNODE *list_search(int row, int col, struct LISTNODE *list_head);

// 在链表中删除一个结点
void list_delete_node(int row, int col, struct LISTNODE **plist_head, struct LISTNODE **plist_tail);

// 检查序列中的重复元素, 将其去除
void list_check(struct LISTNODE **plist_head, struct LISTNODE **plist_tail);

// 复制链表, 返回指向副本首位置的指针
struct LISTNODE *list_clone(struct LISTNODE *list_head);

// 给出链表头, 寻找其尾
struct LISTNODE *list_find_tail(struct LISTNODE *list_head);

// 检查一个格点是否为需要考虑的位置, 即周围8个位置是不是有有子
int check_position(int row, int col);

// 将玩家/机器的落子记录下来的一系列操作, 得到的很多结果用的都是全局变量
void record_position(int round, int player_turn, int row, int col);

// 对链表中的可选项进行排序的函数
int sort_optional_points(struct ELEMENT sorted_options[TREE_WIDTH], struct LISTNODE *list_head, int player_turn);

// 最终进行的棋谱记录
int final_recording(int game_mode, int first_player, int round, int winner);

// 得到struct tm *格式的时间
struct tm *get_time_tm();

// 打印棋盘到文件
void fprint_board(int board[BOARD_SIZE][BOARD_SIZE], FILE *fp);

/*
    Below are functions used for debugging. Only use print_twotree
*/

// // 打印链表所有结点
// void print_list(struct LISTNODE *root);

// // 打印树所有结点, 中序遍历
// void print_tree(struct TNODE *root);

// // 打印树的第order个子节点下的子树
// void print_tchild(struct TNODE *root, int order);

// 打印以一结点为根的某树的向下两层
void print_twotree(struct TNODE *tree_head);

/*
    全局变量声明区域
*/

extern int inner_board_now[BOARD_SIZE][BOARD_SIZE]; // 记录棋局用的棋盘

extern int col_now_global;                     // 刚才下的棋子的行位置
extern int row_now_global;                     // 刚才下的棋子的列位置
extern int row_black_global;                   // 刚才下的黑棋的行位置
extern int col_black_global;                   // 刚才下的黑棋的列位置
extern int row_white_global;                   // 刚才下的白棋的行位置
extern int col_white_global;                   // 刚才下的白棋的列位置
extern int row_black_global_back[MAX_RETRACT]; // 再之前下的黑棋的行位置, 数字越大对应的棋越早
extern int col_black_global_back[MAX_RETRACT]; // 再之前下的黑棋的列位置, 数字越大对应的棋越早
extern int row_white_global_back[MAX_RETRACT]; // 再之前下的白棋的行位置, 数字越大对应的棋越早
extern int col_white_global_back[MAX_RETRACT]; // 再之前下的白棋的列位置, 数字越大对应的棋越早
extern int retract_times_left_black;           // 剩余悔棋次数
extern int retract_times_left_white;           // 剩余悔棋次数

extern const char play1_pic[];         // 之前下的黑棋子
extern const char play1_current_pic[]; // 刚才下的黑棋子
extern const char play2_pic[];         // 之前下的白棋子
extern const char play2_current_pic[]; // 刚才下的白棋子

extern struct POSITION game_record[];  // 棋谱记录
extern int first_layer_max_used_width; // 第一层用到的最靠后的结点, 用于最终棋谱的生成

extern int tuple_set_hori[BOARD_SIZE][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 横向
extern int tuple_set_topr[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 左下-右上
extern int tuple_set_vert[TUPLE_NUMB][BOARD_SIZE][TUPLE_SIZE]; // 打分用的六元组: 纵向
extern int tuple_set_topl[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分用的六元组: 左上-右下

extern int point_hori[BOARD_SIZE][TUPLE_NUMB]; // horizontal   打分六元组: 横向
extern int point_topr[TUPLE_NUMB][TUPLE_NUMB]; // top_right    打分六元组: 左下-右上
extern int point_vert[TUPLE_NUMB][BOARD_SIZE]; // vertical     打分六元组: 纵向
extern int point_topl[TUPLE_NUMB][TUPLE_NUMB]; // top_left     打分六元组: 左上-右下

// 方向棋盘变动时, row和col加减的数 (方向矢量), 就是一个参数数组
extern const int row_sign[8];
extern const int col_sign[8];

extern struct TNODE *root; // 全局通用的根节点, 因为每次只有一个树

extern int chessboard_copy[BOARD_SIZE][BOARD_SIZE]; // 建立一个棋盘的副本, 用在机器落子中

extern int tuple_copy_hori[BOARD_SIZE][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 横向
extern int tuple_copy_topr[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 左下-右上
extern int tuple_copy_vert[TUPLE_NUMB][BOARD_SIZE][TUPLE_SIZE]; // 打分六元组副本: 纵向
extern int tuple_copy_topl[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 左上-右下

extern int point_copy_hori[BOARD_SIZE][TUPLE_NUMB]; // horizontal   打分六元组副本: 横向
extern int point_copy_topr[TUPLE_NUMB][TUPLE_NUMB]; // top_right    打分六元组副本: 左下-右上
extern int point_copy_vert[TUPLE_NUMB][BOARD_SIZE]; // vertical     打分六元组副本: 纵向
extern int point_copy_topl[TUPLE_NUMB][TUPLE_NUMB]; // top_left     打分六元组副本: 左上-右下

#endif
