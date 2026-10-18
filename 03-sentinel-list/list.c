#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  用【哨兵节点】重新实现这 9 个函数。
 *
 *  你会发现的几件事：
 *    · push_front / push_back 不用再特判空链表了
 *    · remove 不用再特判"删的是头节点"了
 *    · reverse 更简单
 *    · 大部分函数都不需要 Node ** 了
 *
 *  建议：写完对比一下两个版本的代码行数。
 * ============================================================ */


/* ---------- 1. 创建链表（含哨兵）---------- */
Node *list_create(void)
{
    Node* p=(Node*)malloc(sizeof(Node));
    p->next=NULL;
    return p;
    /* TODO: 
       1. malloc 一个节点（这就是哨兵）
       2. 把它的 next 置为 NULL（表示"还没有数据节点"）
       3. 返回它
       ⚠️ 哨兵的 data 不用管（是废的）
    */
    return NULL;
}


/* ---------- 2. 头插 ---------- */
void list_push_front(Node *head, int val)
{
    Node* n=(Node*)malloc(sizeof(Node));
    n->data=val;
    n->next=head->next;
    head->next=n;

    /* TODO: 三步
       1. 造新节点 n
       2. n->next = head->next;      ← 指向"原来的第一个数据节点"
       3. head->next = n;            ← 哨兵的 next 指向新节点
       ⚠️ 不用特判空链表！head 永远存在，head->next 是 NULL 也没关系
    */
}


/* ---------- 3. 尾插 ---------- */
void list_push_back(Node *head, int val)
{
    Node* n=(Node*)malloc(sizeof(Node));
    n->data=val;
    n->next=NULL;
    Node* tail=head;
    while(tail->next!=NULL)
    {
        tail=tail->next;
    }
    tail->next=n;
    /* TODO:
       1. 造新节点 n，n->next = NULL
       2. 从 head 出发，走到最后一个节点（next 是 NULL 的那个）
       3. 把它的 next 指向 n
       ⚠️ 不用特判空链表！空链表时 head->next 是 NULL，走一步都不到，
          直接就是 p = head，然后 head->next = n
    */
}


/* ---------- 4. 节点个数（不算哨兵）---------- */
int list_size(Node *head)
{
    int i=0;
    Node* tail=head;
    while(tail->next!=NULL)
    {
        i++;
        tail=tail->next;
    }
    /* TODO: 从 head->next 开始数（别把哨兵数进去） */
    return i;
}


/* ---------- 5. 打印（从 head->next 开始）---------- */
void list_print(Node *head)
{
    Node* tail=head;
    while(tail->next!=NULL)
    {
        printf("%d -> ",tail->next->data);
        tail=tail->next;
    }
    printf("NULL");
    /* TODO: 从 head->next 开始遍历，格式：10 -> 20 -> 30 -> NULL */
}


/* ---------- 6. 查找 ---------- */
Node *list_find(Node *head, int val)
{
    Node* tail=head;
    while(tail->next!=NULL)
    {
        if(tail->next->data==val)
    {
        return tail->next;
    }
    tail=tail->next;
    }
    /* TODO: 从 head->next 开始找 */
    return NULL;
}


/* ---------- 7. 删除 ---------- */
int list_remove(Node *head, int val)
{
    Node* tail=head;
    while(tail->next!=NULL)
    {
        if(tail->next->data==val)
    {
        Node* old=tail->next;
        tail->next=tail->next->next;
        free(old);
        return 1;
    }
    else
    {
        tail=tail->next;
    }
    }
    /* TODO: 一个循环就够！不需要任何特判
       p = head;
       while (p->next != NULL && p->next->data != val)
           p = p->next;
       if (p->next == NULL) return 0;
       Node *old = p->next;
       p->next = old->next;
       free(old);
       return 1;
       ⚠️ 注意和上一版对比：空链表特判没了、"删头节点"特判也没了
    */
    return 0;
}


/* ---------- 8. 反转（哨兵不动）---------- */
void list_reverse(Node *head)
{
    Node* c=head->next;
    Node* p=NULL;
    Node* n;
    while(c!=NULL)
    {
        n=c->next;
        c->next=p;
        p=c;
        c=n;
    }
    head->next=p;
    /* TODO:
       把 head->next 之后的所有节点反转，哨兵本身不动。
       提示：先取 first = head->next，然后对 first 这条链做三指针反转，
             最后 head->next = 新的头。
       ⚠️ 空链表和单节点都不用特判
    */
}


/* ---------- 9. 释放（含哨兵）---------- */
void list_free(Node **head)
{
   Node* next;
    Node* cur=*head;
    while(cur!=NULL)
    {
        next=cur->next;
        free(cur);
        cur=next;
    }
    *head=NULL;
    /* TODO:
       和上一版一样，把整条链（包括哨兵）都 free 掉，
       最后 *head = NULL
       ⚠️ 别忘了哨兵本身也要 free
    */
}
