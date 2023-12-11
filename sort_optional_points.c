#include "gomoku.h"

// 对链表中的可选项进行排序的函数
int sort_optional_points(struct ELEMENT sorted_options[TREE_WIDTH], struct LISTNODE *list_head, int player_turn)
{
    struct LISTNODE *list_node;
    int index = 0, i, j;
    ll point, point_inv, point_diff;

    for (i = 0; i < TREE_WIDTH; i++) // 初始化数组
    {
        sorted_options[i].col = 0;
        sorted_options[i].row = 0;
        sorted_options[i].point = 0;
    }

    list_node = list_head;
    while (list_node != NULL) // 遍历链表
    {
        point = evaluation(player_turn, list_node->row, list_node->col, 1);                           // 对链表的元素得到打分
        point_inv = evaluation(BLACK + WHITE - player_turn, list_node->row, list_node->col, 0) * 0.4; // 反向打分, 得到的是另一方落子的分值
        point_diff = point - point_inv;                                                               // 这样黑棋得到的仍然应该是尽量大, 白棋仍然是尽量小

        if (player_turn == BLACK)
        {
            i = index - 1;
            while (i >= 0 && sorted_options[i].point_diff < point_diff)
                --i;
            if (i != TREE_WIDTH - 1) // 这说明至少有一个数是比新元素的分值要低的
            {
                for (j = ((index < TREE_WIDTH - 1) ? index : TREE_WIDTH - 1); j > i + 1; j--)
                    sorted_options[j] = sorted_options[j - 1];
                // 将得到的结果放在option里, 准备复制进链表
                sorted_options[i + 1].row = list_node->row;
                sorted_options[i + 1].col = list_node->col;
                sorted_options[i + 1].point = point;
                sorted_options[i + 1].point_diff = point_diff;
            }
        }
        else // 反过来, 选取最低的TREE_WIDTH项
        {
            i = index - 1;
            while (i >= 0 && sorted_options[i].point_diff > point_diff) // 其实只变了这一行
                --i;
            if (i != TREE_WIDTH - 1) // 这说明至少有一个数是比新元素的分值要高的
            {
                for (j = ((index < TREE_WIDTH - 1) ? index : TREE_WIDTH - 1); j > i + 1; j--)
                    sorted_options[j] = sorted_options[j - 1];
                sorted_options[i + 1].row = list_node->row;
                sorted_options[i + 1].col = list_node->col;
                sorted_options[i + 1].point = point;
                sorted_options[i + 1].point_diff = point_diff;
            }
        }

        list_node = list_node->next; // 考虑链表的下一个元素
        if (index < TREE_WIDTH)      // 如果index还不到TREE_WIDTH, 就让它+1 (表示多向数组中存进来了一个元素)
            ++index;
    }

    return index; // 返回数组中存储的元素个数
}
