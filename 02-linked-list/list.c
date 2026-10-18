#include <stdlib.h>
#include<stdio.h>
#include "list.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  实现下面 9 个函数。
 *  难度从易到难，建议按顺序做，做完一个跑一次 run.bat。
 * ============================================================ */


/* ---------- 1. 创建空链表 ---------- */
/* ★ 这个已经给你写好了，不用动 —— 空链表就是"头指针是 NULL" */
Node *list_create(void)
{
    return NULL;
}


/* ---------- 2. 头插 ---------- */
void list_push_front(Node **head, int val)
{
    Node* p=malloc(sizeof(Node));
    if(p==NULL)return;
    p->data=val;
    p->next=*head;
    *head=p;

    /* TODO: 三步
       1. 申请一个新节点：  Node *n = malloc(sizeof(Node));
       2. 填数据：          n->data = val;   n->next = *head;
       3. 让新节点成为头：  *head = n;
       ⚠️ 注意 n->next 要指向"原来的头"，也就是 *head
       ⚠️ 第 3 步改的是 *head（不是 head）
    */
}


/* ---------- 3. 尾插 ---------- */
void list_push_back(Node **head, int val)
{
    Node* last=*head;
    Node* p=malloc(sizeof(Node));
    p->data=val;
    p->next=NULL;
    if(*head==NULL)
    {
        *head=p;
    }
    else
    {
        while(last->next!=NULL)
        {
            last=last->next;
        }
        last->next=p;
    }

    /* TODO: 分两种情况
       情况 A：链表是空的（*head == NULL）→ 新节点就是头
       情况 B：链表非空 → 从头走到最后一个节点，把它的 next 指向新节点
       ⚠️ 走的时候用临时指针，别把 *head 走丢了
    */
}


/* ---------- 4. 节点个数 ---------- */
int list_size(Node *head)
{
    int i=0;
    Node* tail=head;
    if(tail!=NULL)
    {
        while(tail!=NULL)
        {
            tail=tail->next;
            i++;
        }
        return i;
    }
    /* TODO: 从 head 出发，一路 next 到 NULL，边走边数
       提示：for (Node *p = head; p != NULL; p = p->next) n++;
    */
    return 0;
}


/* ---------- 5. 打印链表 ---------- */
void list_print(Node *head)
{
    Node* tail=head;
    while(tail!=NULL)
    {
        printf("%d -> ",tail->data);
        tail=tail->next;
    }
    printf("NULL\n");
    /* TODO: 每个节点打印 "值 -> "，最后打印 "NULL\n"
       格式示例：10 -> 20 -> 30 -> NULL
    */
}


/* ---------- 6. 查找 ---------- */
Node *list_find(Node *head, int val)
{
    Node* tail=head;
    while(tail!=NULL)
    {
        if(tail->data==val)
        {
            return tail;
        }
        else{
            tail=tail->next;
        }
    }
    /* TODO: 从头走到尾，遇到 data == val 就返回那个节点的地址
       走完了还没找到 → return NULL
    */
    return NULL;
}


/* ---------- 7. 删除第一个值为 val 的节点 ---------- */
int list_remove(Node **head, int val)
{
    Node* tail=*head;
    if(tail==NULL)
    {
        return 0;
    }
    else
    {
        if(tail->data==val)
        {
            *head=tail->next;
            free(tail);
            return 1;
        }
        else{
            while(tail->next!=NULL&&tail->next->data!=val)
            {
                tail=tail->next;
            }
            if(tail->next==NULL)
            {
                return 0;
            }
            else{
                Node* p=tail->next;
tail->next=tail->next->next;
free(p);
return 1;
            }
        }
    }
    /* TODO: 这里最难，分三种情况
       情况 A：链表为空 → return 0
       情况 B：要删的是【头节点】→ *head = (*head)->next，然后 free 掉旧的
       情况 C：要删的在中间或末尾
                → 用一个指针 p 走在"要删节点的前一个"位置
                → 找到后： p->next = p->next->next;  然后 free 掉那个节点
       删除成功 return 1，没找到 return 0
       ⚠️ free 之前一定要先把链子接好
    */
    return 0;
}


/* ---------- 8. 反转链表 ---------- */
void list_reverse(Node **head)
{
     if(*head==NULL)
     *head=NULL;
    else
    {
        Node* p=NULL;
        Node* c=*head;
        Node* n;
        while(c!=NULL)
        {
            n=c->next;
            c->next=p;
            p=c;
            c=n;
        }
        *head=p;
    }
    /* TODO: 经典三指针法
       prev = NULL;
       cur  = *head;
       while (cur != NULL) {
           next = cur->next;    ← 先存住下一个
           cur->next = prev;    ← 把箭头反过来
           prev = cur;          ← prev 前进
           cur  = next;         ← cur 前进
       }
       *head = prev;            ← 最后 prev 就是新头
       ⚠️ 这三步的顺序不能乱，想清楚为什么
    */
}


/* ---------- 9. 释放整条链表 ---------- */
void list_free(Node **head)
{
    (void)head;
    Node* next;
    Node* cur=*head;
    while(cur!=NULL)
    {
        next=cur->next;
        free(cur);
        cur=next;
    }
    *head=NULL;    /* TODO:
       cur = *head;
       while (cur != NULL) {
           next = cur->next;    ← 先记住下一个（否则 free 后就找不到了）
           free(cur);
           cur = next;
       }
       *head = NULL;            ← 别忘了置空
       ⚠️ 如果先 free 再取 next，就访问了已释放的内存
    */
}
