#include "gomoku.h"

int chessboard_copy[BOARD_SIZE][BOARD_SIZE]; // 建立一个棋盘的副本, 用在机器落子中

int tuple_copy_hori[BOARD_SIZE][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 横向
int tuple_copy_topr[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 左下-右上
int tuple_copy_vert[TUPLE_NUMB][BOARD_SIZE][TUPLE_SIZE]; // 打分六元组副本: 纵向
int tuple_copy_topl[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]; // 打分六元组副本: 左上-右下

int point_copy_hori[BOARD_SIZE][TUPLE_NUMB]; // horizontal   打分六元组副本: 横向
int point_copy_topr[TUPLE_NUMB][TUPLE_NUMB]; // top_right    打分六元组副本: 左下-右上
int point_copy_vert[TUPLE_NUMB][BOARD_SIZE]; // vertical     打分六元组副本: 纵向
int point_copy_topl[TUPLE_NUMB][TUPLE_NUMB]; // top_left     打分六元组副本: 左上-右下

// 实现机器落子的函数. 需要: 该谁下player_turn, 第几轮round, 棋盘
int computer_placing(int player_turn, int round)
{
    struct TNODE *first_layer; // 建立第一层用的结点
    ll final_score;            // minimax最后的打分
    int child_num = 0;         // 根节点的子节点的个数

    // 对比定式. 用到的是真正的棋盘
    if (beginning_joseki(round, player_turn) == 1)
        return 0;

    // 拷贝棋盘
    copy_inner_board_to_copy(chessboard_copy);

    // 拷贝六元组
    memcpy(tuple_copy_hori, tuple_set_hori, sizeof(int[BOARD_SIZE][TUPLE_NUMB][TUPLE_SIZE]));
    memcpy(tuple_copy_topr, tuple_set_topr, sizeof(int[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]));
    memcpy(tuple_copy_vert, tuple_set_vert, sizeof(int[TUPLE_NUMB][BOARD_SIZE][TUPLE_SIZE]));
    memcpy(tuple_copy_topl, tuple_set_topl, sizeof(int[TUPLE_NUMB][TUPLE_NUMB][TUPLE_SIZE]));

    // 拷贝之前的打分
    memcpy(point_copy_hori, point_hori, sizeof(int[BOARD_SIZE][TUPLE_NUMB]));
    memcpy(point_copy_topr, point_topr, sizeof(int[TUPLE_NUMB][TUPLE_NUMB]));
    memcpy(point_copy_vert, point_vert, sizeof(int[TUPLE_NUMB][BOARD_SIZE]));
    memcpy(point_copy_topl, point_topl, sizeof(int[TUPLE_NUMB][TUPLE_NUMB]));

    // 初始化初始层
    root->child = NULL;
    root->sibling = NULL;
    root->row = -1;
    root->col = -1;
    root->point = 0;

    // 开始进行搜索
    final_score = minimax(NULL, root, TREE_DEPTH, -INFINITY, INFINITY, round, player_turn);

    printf("point: %lld\n", final_score); // show the decision tree
    print_twotree(root);                  // show the decision tree

    first_layer = root->child; // 令第一层指向root的第一个child
    if (first_layer == NULL)
    {
        printf("发生错误，无选点！\n");
        return -1;
    }
    ++child_num; // 计算最终下到了第几个子结点, 用于生成棋谱

    // print_twotree(first_layer); // show the decision tree

    if (first_layer->point != final_score) // 如果这个点不等于最终的结果
    {
        ++child_num;
        if (first_layer->sibling != NULL)       // 如果有子节点
            first_layer = first_layer->sibling; // 指向它的子节点

        // print_twotree(first_layer); // show the decision tree

        while (first_layer->point != final_score && first_layer->sibling != NULL) // 如果这个点不是最终结果, 且还有子节点
        {
            ++child_num;
            first_layer = first_layer->sibling; // 指向它的子节点
            // print_twotree(first_layer); // show the decision tree
        }
    }
    tree_free(root->child);

    printf("\naccept child_node order:%d\n", child_num); // show the decision tree
    if (child_num > first_layer_max_used_width)          // 用于记录
        first_layer_max_used_width = child_num;

    record_position(round, player_turn, first_layer->row, first_layer->col); // 记录落子

    return 0; // 返回选中的结果
}

// minimax法, 对决策树做alpha-beta剪枝
ll minimax(struct LISTNODE *old_list_head, struct TNODE *tree_head, int depth, int alpha, int beta, int round, int player_turn)
{
    static int count = 0;
    ll max_eval;                // 最大值
    ll min_eval;                // 最小值
    ll eval;                    // 打分值
    int row;                    // 当前落子的行
    int col;                    // 当前落子的列
    struct LISTNODE *list_head; // 新建的链表头
    struct TNODE *child_node;   // 用于遍历子节点的一个结构体指针
    ll score;                   // 最后返回的分数
    int judge = 0;              // 判断第一步下去后是不是应该剪枝

    row = tree_head->row; // 赋值: 当前落子的行
    col = tree_head->col; // 赋值: 当前落子的列
    place_stone_on_copy(row, col, player_turn);

    if (depth == TREE_DEPTH)
    {
        list_head = create_first_floor(root, player_turn, round, depth); // 建立初层
        player_turn = BLACK + WHITE - player_turn;
    }
    else
        list_head = create_new_floor(old_list_head, tree_head, row, col, BLACK + WHITE - player_turn, round + 1, depth - 1);

    // 如果没有子节点, 那就返回本身的分数值
    if (tree_head->child == NULL)
    {
        list_free(list_head);
        place_stone_on_copy(row, col, SPARE);
        return tree_head->point;
    }

    // 如果现在正在处于最大层, 需要进行递归

    if (player_turn == WHITE) // 最大层. 本层落白子, 下一层该下黑子, 黑方会选分数最高的一个, 因此最高分就是白子的分
    {
        max_eval = -INFINITY;          // 最大值
        child_node = tree_head->child; // 把新层中head的子节点记录, 开始依次搜索
        while (child_node != NULL)     // 如果子节点非空
        {
            eval = minimax(list_head, child_node, depth - 1, alpha, beta, round + 1, BLACK); // 递归, 得到eval值

            max_eval = max(max_eval, eval);   // 找到eval与maxeval中的较大值
            alpha = max(alpha, eval);         // 找到eval与alpha  中的较大值
            if (beta <= alpha)                // alpha大于等于beta!
                break;                        // 子节点的兄弟可以无需考虑了
            child_node = child_node->sibling; // 重复一遍上面的过程
        }
        tree_head->point = max_eval;
        list_free(list_head);
        place_stone_on_copy(row, col, SPARE);
        return max_eval;
    }

    // 如果现在正在处于最小层, 需要进行递归

    else // 最小层. 本层落黑子, 下一层该下白子, 白方会选分数最低的一个, 因此最低分就是黑子的分
    {
        min_eval = INFINITY;
        child_node = tree_head->child;
        while (child_node != NULL)
        {
            eval = minimax(list_head, child_node, depth - 1, alpha, beta, round + 1, WHITE);

            min_eval = min(min_eval, eval);
            beta = min(beta, eval);
            if (beta <= alpha)
                break;
            child_node = child_node->sibling;
        }
        tree_head->point = min_eval;
        list_free(list_head);
        place_stone_on_copy(row, col, SPARE);
        return min_eval;
    }
}
