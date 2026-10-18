#ifndef LIST_H
#define LIST_H

/* ============================================================
 *  单链表 —— 函数声明
 *  这个文件不要动！
 * ============================================================ */

typedef struct Node {
    int data;
    struct Node *next;
} Node;

/* 1. 创建一个空链表（其实就是返回 NULL） */
Node *list_create(void);

/* 2. 头插：把 val 插到链表最前面
      ⚠️ 参数是 Node **head —— 因为可能要改 head 本身 */
void list_push_front(Node **head, int val);

/* 3. 尾插：把 val 加到链表最后面 */
void list_push_back(Node **head, int val);

/* 4. 返回节点个数 */
int list_size(Node *head);

/* 5. 打印整条链表，格式：10 -> 20 -> 30 -> NULL */
void list_print(Node *head);

/* 6. 查找第一个值为 val 的节点，返回它的地址；找不到返回 NULL */
Node *list_find(Node *head, int val);

/* 7. 删除第一个值为 val 的节点。删掉了返回 1，没找到返回 0 */
int list_remove(Node **head, int val);

/* 8. 原地反转整条链表（改指针，不搬数据） */
void list_reverse(Node **head);

/* 9. 释放整条链表，并把 *head 置为 NULL */
void list_free(Node **head);

#endif  /* LIST_H */
