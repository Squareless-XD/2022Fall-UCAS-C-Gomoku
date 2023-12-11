#include "gomoku.h"

// 给链表新增元素/增加出现次数
int list_set_new(int row, int col, int add_to_head, struct LISTNODE **plist_head, struct LISTNODE **plist_tail)
{
    struct LISTNODE *list_node = NULL; // 旧的子节点

    if (*plist_head == NULL)
    {
        list_set_head(row, col, plist_head, plist_tail); // 设置一个新的链表结点作为开头
        return 0;
    }

    if ((*plist_head)->row == row && (*plist_head)->col == col)
        return 1; // 已经存在

    list_node = *plist_head;
    while (list_node->next != NULL)
    {
        list_node = list_node->next;
        if (list_node->row == row && list_node->col == col)
            return 1; // 已经存在
    }

    // 以下开始新建一个结点. 这时list_node->next应该是一个空指针

    if (add_to_head == ADD_TO_LIST_HEAD) // 如果要新建在最前面
        list_add_head(row, col, plist_head);
    else // 新建在最后面
        list_add_tail(row, col, plist_tail);
    return 0;
}

// 给一个空的链表加一个开头
void list_set_head(int row, int col, struct LISTNODE **plist_head, struct LISTNODE **plist_tail)
{
    struct LISTNODE *new_node = NULL;

    new_node = lalloc(); // 开一块区域

    // 赋值
    new_node->row = row;
    new_node->col = col;
    new_node->next = NULL;

    // 开头和结尾都指向新节点
    *plist_head = new_node;
    *plist_tail = new_node;
}

// 新建一个LISTNODE
struct LISTNODE *lalloc(void)
{
    return (struct LISTNODE *)malloc(sizeof(struct LISTNODE));
}

// 将链表的结点全部释放
void list_free(struct LISTNODE *list_head)
{
    struct LISTNODE *list_node = list_head, *clear_node;
    while (list_node != NULL) // 当遇到的结点非空时
    {
        clear_node = list_node;      // 将这个结点的坐标存下来
        list_node = list_node->next; // 移动到其下一个结点
        free(clear_node);            // 释放掉存储的这个节点的坐标
    }
}

// 给链表开头加一个结点
void list_add_head(int row, int col, struct LISTNODE **plist_head)
{
    struct LISTNODE *new_node;

    new_node = lalloc(); // 开一块区域

    // 赋值
    new_node->row = row;
    new_node->col = col;
    new_node->next = *plist_head;

    *plist_head = new_node; // 重置头结点
}

// 给链表结尾加一个结点
void list_add_tail(int row, int col, struct LISTNODE **plist_tail)
{
    struct LISTNODE *new_node;

    new_node = lalloc(); // 开一块区域

    // 赋值
    new_node->row = row;
    new_node->col = col;
    new_node->next = NULL;

    (*plist_tail)->next = new_node; // 连上一个节点
    *plist_tail = new_node;         // 重置尾结点
}

// 在链表中搜索一个结点
struct LISTNODE *list_search(int row, int col, struct LISTNODE *list_head)
{
    struct LISTNODE *list_node = list_head; // 旧的子节点

    if (list_node == NULL)
        return NULL; // 链表是空链表
    while (list_node != NULL && (list_node->row != row || list_node->col != col))
        list_node = list_node->next;
    if (list_node != NULL)
        return list_node; // 存在要搜索的结点
    return NULL;          // 不存在要搜索的结点
}

// 在链表中删除一个结点
void list_delete_node(int row, int col, struct LISTNODE **plist_head, struct LISTNODE **plist_tail)
{
    struct LISTNODE *list_node, *next_node;
    if (*plist_head == NULL)
        return;
    if ((*plist_head)->row == row && (*plist_head)->col == col)
    {
        list_node = *plist_head;
        *plist_head = (*plist_head)->next;
        free(list_node);
    }
    list_node = *plist_head;
    while (list_node->next != NULL && (list_node->next->row != row || list_node->next->col != col))
        list_node = list_node->next;
    if (list_node->next != NULL)
    {
        list_node->next = list_node->next->next;
        if (*plist_tail == list_node->next)
            *plist_tail = list_node;
        free(list_node->next);
    }
}

// 复制链表, 返回指向副本首位置的指针
struct LISTNODE *list_clone(struct LISTNODE *list_head)
{
    struct LISTNODE *list_node = list_head;
    struct LISTNODE *copy_head, *copy_node, *copy_temp;
    if (list_node != NULL)
    {
        copy_head = lalloc();
        copy_head->row = list_node->row;
        copy_head->col = list_node->col;
        copy_head->next = NULL;
        copy_node = copy_head;
        list_node = list_node->next;
    }
    while (list_node != NULL)
    {
        copy_temp = lalloc();
        copy_temp->row = list_node->row;
        copy_temp->col = list_node->col;
        copy_temp->next = NULL;
        copy_node->next = copy_temp;
        copy_node = copy_temp;
        list_node = list_node->next;
    }
    return copy_head;
}

// 给出链表头, 寻找其尾
struct LISTNODE *list_find_tail(struct LISTNODE *list_head)
{
    struct LISTNODE *list_node = list_head;
    if (list_node == NULL)
        return NULL;
    while (list_node->next != NULL)
        list_node = list_node->next;
    return list_node;
}

// // 打印链表所有结点
// void print_list(struct LISTNODE *list_head)
// {
//     if (list_head == NULL)
//         return;
//     printf("%c%d\n", 'A' + list_head->col, 15 - list_head->row);
//     print_list(list_head->next);
// }
