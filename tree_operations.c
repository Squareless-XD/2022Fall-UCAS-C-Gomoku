#include "gomoku.h"

// 给树建立一个新的孩子, 自动选择是在child处还是在child的sibling处建立
int mytree_set_child(struct TNODE *node, int row, int col, ll point)
{
    struct TNODE *child_node = NULL; // 旧的子节点, TNODE
    struct TNODE *new_node = NULL;   // 新的子节点, TNODE
    int order_of_children = 0;

    new_node = talloc();      // 开一块区域
    new_node->point = point;  // 赋值
    new_node->row = row;      // 赋值
    new_node->col = col;      // 赋值
    new_node->child = NULL;   // 赋值
    new_node->sibling = NULL; // 赋值

    if (node->child == NULL) // 如果没有第一个孩子
    {
        node->child = new_node; // 连上第一个孩子
        return 0;
    }
    else // 如果有第一个孩子
    {
        child_node = node->child;           // 记录一下结点的孩子
        while (child_node->sibling != NULL) // 通过一串while操作找到最后一个孩子
        {
            child_node = child_node->sibling;
            order_of_children++;
        }
        child_node->sibling = new_node;
    }
    return order_of_children; // 返回第几个sibling, 即原本有几个child, 即新的子节点从0开始"存储的位置"
}

// 给树的根节点刷新
int mytree_set_root(void)
{
    struct TNODE *new_node = NULL; // 新的子节点, TNODE

    new_node = talloc();  // 开一块区域
    if (new_node == NULL) // 如果开不了区域
    {
        printf("mytree_set_root: talloc() failed.\n"); // 报错
        return -1;
    }
    root = new_node; // 根指向新节点
    return 0;
}

// 新建一个TNODE
struct TNODE *talloc(void)
{
    return (struct TNODE *)malloc(sizeof(struct TNODE));
}

// 新建一个ELEMENT
struct ELEMENT *ealloc(void)
{
    return (struct ELEMENT *)malloc(sizeof(struct ELEMENT));
}

// 将树的结点全部释放
void tree_free(struct TNODE *thead)
{
    // 链表头为空, 就直接返回
    if (thead == NULL)
        return;
    tree_free(thead->child);   // 递归释放子节点
    tree_free(thead->sibling); // 递归释放兄弟结点
    free(thead);               // 返还本身的内存
}

// // 打印树所有结点, 中序遍历
// void print_tree(struct TNODE *root)
// {
//     static int order = 0;
//     if (root == NULL)
//         return;
//     printf("%d:  %c%02d point:%lld\n", order, 'A' + root->col, BOARD_SIZE - root->row, root->point);
//     ++order;
//     print_tree(root->child);
//     --order;
//     print_tree(root->sibling);
// }

// // 打印树的第order个子节点下的子树
// void print_tchild(struct TNODE *root, int order)
// {
//     if (order == 0)
//     {
//         print_tree(root);
//         return;
//     }
//     --order;
//     print_tchild(root->sibling, order);
// }

// 打印树的两层
void print_twotree(struct TNODE *tree_head)
{
    // 没什么好说的, 就是往下打印两层. 这个函数本身的意义并不大, 就是给下棋的玩家看着用的
    struct TNODE *tree_node, *tree_node2;
    int count;
    if (root == NULL)
        return;
    printf("0: %c%02d point:%lld\n", 'A' + root->col, BOARD_SIZE - root->row, root->point);
    tree_node = root->child;
    while (tree_node != NULL)
    {
        printf("\n 1: %c%02d point:%lld\n", 'A' + tree_node->col, BOARD_SIZE - tree_node->row, tree_node->point);
        tree_node2 = tree_node->child;
        count = 0;
        while (tree_node2 != NULL)
        {
            if (++count % 4 == 0)
                printf("  2: %c%02d point:%lld\n", 'A' + tree_node2->col, BOARD_SIZE - tree_node2->row, tree_node2->point);
            else
                printf("  2: %c%02d point:%lld", 'A' + tree_node2->col, BOARD_SIZE - tree_node2->row, tree_node2->point);
            tree_node2 = tree_node2->sibling;
        }
        tree_node = tree_node->sibling;
    }
    printf("\n");
}
