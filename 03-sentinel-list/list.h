#ifndef LIST_H
#define LIST_H

/* ============================================================
 *  单链表（哨兵版）—— 函数声明
 *
 *  ⚠️ 和上一版最大的区别：
 *     1. list_create() 会 malloc 一个【哨兵节点】，永远不返回 NULL
 *     2. 所有函数参数从 Node ** 变成了 Node *（除了 list_free）
 *     3. 真正的数据从 head->next 开始
 * ============================================================ */

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* 1. 创建链表 —— 会 malloc 一个哨兵节点
      返回：哨兵节点的地址（永远不为 NULL）
      ⚠️ 哨兵节点的 data 是废的，真正的数据在 head->next 之后 */
Node *list_create(void);

/* 2. 头插：把 val 插到【第一个数据节点】前面
      ⚠️ 注意：不是插在 head 前面，是插在 head->next 前面 */
void list_push_front(Node *head, int val);

/* 3. 尾插：把 val 加到链表最后面 */
void list_push_back(Node *head, int val);

/* 4. 返回【数据节点】的个数（不算哨兵） */
int list_size(Node *head);

/* 5. 打印从 head->next 开始的所有数据 */
void list_print(Node *head);

/* 6. 查找第一个值为 val 的数据节点 */
Node *list_find(Node *head, int val);

/* 7. 删除第一个值为 val 的数据节点。删掉了返回 1，没找到返回 0
      ⚠️ 注意哨兵永远不会被删，所以不用特判头节点！ */
int list_remove(Node *head, int val);

/* 8. 原地反转【数据节点】（哨兵位置不变） */
void list_reverse(Node *head);

/* 9. 释放整条链表（含哨兵），并把 *head 置为 NULL
      ⚠️ 这个还是需要 Node **，因为要把调用方的 head 置空 */
void list_free(Node **head);

#endif  /* LIST_H */
