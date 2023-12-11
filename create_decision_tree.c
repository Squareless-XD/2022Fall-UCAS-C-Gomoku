#include "gomoku.h"

// // 建立决策树
// void build_decision_tree(int player_turn, int round)
// {
//     // 初始化初始层
//     root->child = NULL;
//     root->sibling = NULL;
//     root->row = -1;
//     root->col = -1;
//     root->point = 0;
//     create_first_floor(root, row_now_global, col_now_global, player_turn, round, TREE_DEPTH);
// }

// 建立第一层新的可能性, 仅限第一层
struct LISTNODE *create_first_floor(struct TNODE *tree_head, int player_turn, int round, int depth)
{
    // depth 表示的是递归进行几层. depth为0则不需要再添加新层, 直接结束.

    struct LISTNODE *list_head = NULL;
    struct LISTNODE *list_tail = NULL;
    int row_temp, col_temp;
    int i, j;
    int judge;
    struct ELEMENT sorted_options[TREE_WIDTH]; // 将最终选出的TREE_WIDTH个点拉出来, 每一个位置分别存储row和col和point. 注意这是带排序的
    int option_num;                            // 记录一共有多少个点被选中
    int chess_board_another[BOARD_SIZE][BOARD_SIZE];

    // list_free(list_head, list_tail);

    printf("create_first_floor: executing.\n"); // debug

    for (i = 0; i < BOARD_SIZE; i++)
        for (j = 0; j < BOARD_SIZE; j++)
            if (chessboard_copy[i][j] == SPARE) // 棋盘的(i,j)位置为空
            {
                judge = check_position(i, j);
                if (judge == 1)
                    list_set_new(i, j, ADD_TO_LIST_HEAD, &list_head, &list_tail);
                else if (judge == 2)
                    list_set_new(i, j, ADD_TO_LIST_TAIL, &list_head, &list_tail);
            }

    if (list_head == NULL) // 空链表, 意味着没有点, 先直接结束试试
    {
        printf("没有选点可以选!\n");
        return NULL;
    }

    // 再拷贝一个棋盘, 因为是第一层所以可以直接用原本的棋盘判断是不是胜利手
    copy_inner_board_to_copy(chess_board_another);

    // 将链表中可选的结果择优选择TREE_WIDTH项, 从高分到低分排列
    option_num = sort_optional_points(sorted_options, list_head, player_turn);

    // 将TREE_WIDTH个需要考虑的位置写入决策树根的子节点
    for (i = 0; i < option_num; i++)
    {
        if (judge_winning(player_turn, sorted_options[i].row, sorted_options[i].col, chess_board_another))
            sorted_options[i].point = (player_turn == BLACK) ? (2 * POINT_WIN) : (-2 * POINT_WIN);
        mytree_set_child(tree_head, sorted_options[i].row, sorted_options[i].col, sorted_options[i].point);
    }

    return list_head;
    // // 然后对每一个子结点, 落子, 递归地建立新层, 取消落子, 干这么一条龙. 注意建立新层需要传递链表头(尾通过函数取得)
    // tree_node = tree_head->child;
    // for (i = 0; i < option_num; i++)
    // {
    //     place_stone_on_copy(tree_node->row, tree_node->col, player_turn);
    //     create_new_floor(list_head, tree_node, tree_node->row, tree_node->col, BLACK + WHITE - player_turn, round + 1, depth - 1);
    //     place_stone_on_copy(tree_node->row, tree_node->col, SPARE);
    //     tree_node = tree_node->sibling;
    // }
}

// 建立一层新的可能性
struct LISTNODE *create_new_floor(struct LISTNODE *old_list_head, struct TNODE *tree_head, int row, int col, int player_turn, int round, int depth)
{
    // int empty_board[BOARD_SIZE][BOARD_SIZE];    // 空棋盘

    // 如果深度为0, 也就是说之前已经建立了五层, 那么这里没有决策树的事儿了
    if (depth == 0)
        return NULL;

    // 如果上一步棋就结束游戏了 (之前打过分的), 那就不再继续向下探索了
    if (tree_head->point >= POINT_WIN - 100000 || tree_head->point <= 100000 - POINT_WIN)
        return NULL;

    // 如果已经没处可下了, 这意味着前BOARD_SIZE^2轮已经下完了, 那么我们认为这盘棋已经结束了
    if (round == BOARD_SIZE * BOARD_SIZE + 1)
        return NULL;

    struct LISTNODE *list_head, *list_tail;
    int row_temp, col_temp;
    int i;
    struct ELEMENT sorted_options[TREE_WIDTH]; // 将最终选出的TREE_WIDTH个点拉出来, 每一个位置分别存储row和col和point. 注意这是带排序的
    int option_num;

    list_head = list_clone(old_list_head);
    list_tail = list_find_tail(list_head);

    // 之前我们已经递归地摆放上上一轮要下的棋子了, 现在我们直接检查链表的更新
    list_check(&list_head, &list_tail);

    // 然后把需要考虑的点(row,col)周围的8*2个点加入链表
    for (i = 0; i < DIRECTION; i++)
    {
        row_temp = row + row_sign[i];
        col_temp = col + col_sign[i];
        if (judge_in_bounds(row_temp, col_temp) && chessboard_copy[row_temp][col_temp] == SPARE)
            list_set_new(row_temp, col_temp, ADD_TO_LIST_HEAD, &list_head, &list_tail);
        row_temp = row + 2 * row_sign[i];
        col_temp = col + 2 * col_sign[i];
        if (judge_in_bounds(row_temp, col_temp) && chessboard_copy[row_temp][col_temp] == SPARE)
            list_set_new(row_temp, col_temp, ADD_TO_LIST_TAIL, &list_head, &list_tail);
    }

    // 这之后如果
    if (list_head == NULL) // 空链表, 意味着没有点, 先直接结束试试
    {
        printf("没有选点可以选!\n");
        return NULL;
    }

    // 将链表中可选的结果择优选择TREE_WIDTH项, 从高分到低分排列. 注意这里的打分需要带全套打分机制, 全都靠这个打分了.
    option_num = sort_optional_points(sorted_options, list_head, player_turn);

    // 将TREE_WIDTH个需要考虑的位置写入决策树根的子节点
    for (i = 0; i < option_num; i++)
        mytree_set_child(tree_head, sorted_options[i].row, sorted_options[i].col, sorted_options[i].point);

    // // 然后对每一个子结点, 落子, 递归地建立新层, 取消落子, 干这么一条龙. 注意建立新层需要传递链表头(尾通过函数取得)
    // tree_node = tree_head->child;
    // for (i = 0; i < option_num; i++)
    // {
    //     place_stone_on_copy(tree_node->row, tree_node->col, player_turn);
    //     create_new_floor(list_head, tree_node, tree_node->row, tree_node->col, BLACK + WHITE - player_turn, round + 1, depth - 1);
    //     place_stone_on_copy(tree_node->row, tree_node->col, SPARE);
    //     tree_node = tree_node->sibling;
    // }

    // list_free(&list_head, &list_tail);

    return list_head;
}

// 检查序列中的重复元素, 将其去除
void list_check(struct LISTNODE **list_head, struct LISTNODE **list_tail)
{
    int clear_head = 0;
    struct LISTNODE *list_node = *list_head, *node_temp;
    int row, col;

    // 如果就是个空链表, 那么直接不做动作
    if (list_node == NULL)
        return;

    // 检查链表头元素该不该去掉了: 上面有子, 或者位置不符合周围两个位置有子 (返回0)
    if (chessboard_copy[list_node->row][list_node->col] != SPARE || check_position(list_node->row, list_node->col) == 0)
        clear_head = 1; // 标记清除首结点

    while (list_node->next != NULL)
    {
        row = list_node->next->row;
        col = list_node->next->col;
        if (chessboard_copy[row][col] != SPARE || check_position(row, col) == 0) // 中间结点不满足要求
        {
            node_temp = list_node->next;
            list_node->next = list_node->next->next;
            free(node_temp);
            if (node_temp == *list_tail) // 如果这个不满足要求的结点是尾结点
            {
                *list_tail = list_node;
                break;
            }
        }
        list_node = list_node->next;
    }
    if (clear_head == 1) // 清理一下应该删掉的首节点
    {
        node_temp = *list_head;
        *list_head = (*list_head)->next;
        free(node_temp);
    }
}

// 检查一个格点是否为需要考虑的位置, 即周围8*2个位置是不是有有子
int check_position(int row, int col)
{
    int k;
    int judge;
    int row_temp, col_temp;

    for (k = 0; k < DIRECTION; k++) // 看八个方向
    {
        row_temp = row + row_sign[k]; // 行向外移动一格
        col_temp = col + col_sign[k]; // 列向外移动一格
        // 如果这一格在界内, 且有子, 那么返回1
        if (judge_in_bounds(row_temp, col_temp) == IN_BOUND && chessboard_copy[row_temp][col_temp] != SPARE)
            return 1;
    }
    for (k = 0; k < DIRECTION; k++) // 同上
    {
        row_temp = row + 2 * row_sign[k];
        col_temp = col + 2 * col_sign[k];
        if (judge_in_bounds(row_temp, col_temp) == IN_BOUND && chessboard_copy[row_temp][col_temp] != SPARE)
            return 2;
    }
    return 0;
}
