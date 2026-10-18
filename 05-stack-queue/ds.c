#include <stdlib.h>
#include <stdio.h>
#include "ds.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  15 个函数，分三部分：
 *      Part A  栈（动态数组实现）
 *      Part B  队列（链表实现）
 *      Part C  括号匹配（用栈）
 *
 *  大部分函数都很短。建议顺序：
 *      stack_create → stack_size/empty → stack_push → stack_pop → stack_top
 *      → stack_free → queue_* → brackets_match
 * ============================================================ */


/* ============================================================
 *  Part A：栈
 * ============================================================ */

/* A1. 创建空栈 */
Stack *stack_create(void)
{
    Stack* p=malloc(sizeof(Stack));
    if(p==NULL)return NULL;
    p->data=malloc(4*sizeof(int));
    if(p->data==NULL)
    {
        free(p);
        return  NULL;
    }
    p->capacity=4;
    p->size=0;
    return p;
    /* TODO: 和 da_create 一模一样（初始容量 4，两次 malloc 都判空） */
}

/* A2. 入栈 */
void stack_push(Stack *s, int val)
{
    if(s->size==s->capacity)
    {
        int newcup=2*s->capacity;
        int* tmp=realloc(s->data,sizeof(int)*newcup);
        if(tmp==NULL)
        {
            return;
        }
        s->data=tmp;
        s->capacity=newcup;
    }
    s->data[s->size]=val;
    s->size++;
    /* TODO: 和 da_push 一模一样
       ① 满了就扩容（容量翻倍，realloc 先存 tmp）
       ② data[size] = val;  size++;
       ⚠️ 栈顶就是最后一个元素，所以【入栈 = 往数组末尾加】
    */
}

/* A3. 出栈 */
int stack_pop(Stack *s)
{
    if(s->size==0)
    {
        return -1;
    }
    s->size--;
    return s->data[s->size];

    /* TODO:
       if (s->size == 0) return -1;      ← 空栈保护
       s->size--;                         ← 先减
       return s->data[s->size];           ← 再取（现在 size 指向"原来的栈顶"）
       ⚠️ 顺序想清楚：先减再取，还是先取再减？
    */
    return -1;
}

/* A4. 看栈顶 */
int stack_top(Stack *s)
{
    if(s->size==0)
    {return -1;}
else{
    return s->data[s->size-1];
}
    /* TODO: 空栈返回 -1，否则返回 data[size - 1] */
}

/* A5. 栈空吗 */
int stack_empty(Stack *s)
{
if(s->size==0)
{
    return 1;
}
else{    /* TODO: size == 0 返回 1，否则 0 */
    return 0;
}
}
/* A6. 元素个数 */
int stack_size(Stack *s)
{
    if(s->size==0)
    return 0;
    else{
        return s->size;
    }
}

/* A7. 释放栈 */
void stack_free(Stack *s)
{
    if(s==NULL)
    return ;
else
{
    free(s->data);
    free(s);
}
    /* TODO: if (s == NULL) return;  free(s->data);  free(s); */
}


/* ============================================================
 *  Part B：队列
 * ============================================================ */

/* B1. 创建空队列 */
Queue *queue_create(void)
{
    Queue* p=malloc(sizeof(Queue));
    p->front=p->rear=NULL;
    p->size=0;
    /* TODO: malloc 一个 Queue，front = rear = NULL，size = 0
       ⚠️ 队列内部【没有数组】，节点是后面入队时一个个 malloc 的
       ⚠️ 判空 */
    return p;
}

/* B2. 入队（加到队尾） */
void queue_push(Queue *q, int val)
{
    QNode* n=malloc(sizeof(QNode));
    n->data=val;
    n->next=NULL;
    if(q->rear==NULL)
    {
        q->rear=n;
        q->front=n;
    }
    else{
        q->rear->next=n;
q->rear=n;
    }
    q->size++;
    /* TODO:
       ① 造新节点 n（n->data = val; n->next = NULL;）
       ② 如果队列是空的（q->rear == NULL）
             q->front = q->rear = n;
          否则
             q->rear->next = n;      ← 旧队尾指向新节点
             q->rear = n;            ← 新节点成为新队尾
       ③ q->size++;
       ⚠️ 记住队尾指针，就不用每次从前往后走一遍了（O(1) 入队）
    */
}

/* B3. 出队（从队头拿走） */
int queue_pop(Queue *q)
{
    if(q->front == NULL)
    {return -1;}
    else{
    int outdata=q->front->data;
    QNode* out=q->front;
    q->front=q->front->next;
    free(out);
    q->size--;
    if(q->front==NULL)
    {
        q->rear=NULL;
    }
    return outdata;}
    /* TODO:
       if (q->front == NULL) return -1;      ← 空队列
       int val = q->front->data;
       QNode *old = q->front;
       q->front = old->next;
       free(old);                             ← 先接链子再 free（老规矩）
       q->size--;
       if (q->front == NULL) q->rear = NULL; ← ⚠️ 队列空了，rear 也要清空！
       return val;
       ⚠️ 最后这一步最容易忘：如果不把 rear 置空，下次入队会写到已释放的节点上
    */
}

/* B4. 看队头 */
int queue_front(Queue *q)
{
    /* TODO: 空队列返回 -1，否则返回 q->front->data */
    if(q->front==NULL)
    return -1;
else{
    return q->front->data;
}
}

/* B5. 队列空吗 */
int queue_empty(Queue *q)
{
    /* TODO: front == NULL 返回 1 */
    if(q->front==NULL)
    return 1;
else return 0;
}

/* B6. 元素个数 */
int queue_size(Queue *q)
{
    if(q->front!=NULL)
    {
        return q->size;
    }
    else
    return 0;
}

/* B7. 释放队列 */
void queue_free(Queue *q)
{
    while(q->front!=NULL)
    {
        QNode* out=q->front;
        q->front=q->front->next;
        free(out);
    }
    q->rear=NULL;
    free(q);
    /* TODO:
       if (q == NULL) return;
       从头到尾把每个节点 free 掉（和 list_free 一样，先存 next 再 free）
       最后 free(q);
    */
}


/* ============================================================
 *  Part C：括号匹配
 * ============================================================ */

int brackets_match(const char *s)
{
    int a=0;
    int b=0;
    Stack* st=stack_create();
    for(int i=0;s[i]!='\0';i++)
    {
        if(s[i]=='(')
        {
            a++;
                stack_push(st,s[i]);
        }
        else if(s[i]=='[')
        {
            a++;
                stack_push(st,s[i]);
        }
        else if(s[i]=='{')
        {
            a++;
                stack_push(st,s[i]);
        }
        else if(s[i]==')')
        {
            b++;
            char g=stack_pop(st);
            if(g!='(')
            {
                return 0;
            }
        }
        else if(s[i]=='}')
        {
            b++;
            char g=stack_pop(st);
            if(g!='{')
            {
                return 0;
            }
        }
        else if(s[i]==']')
        {
            b++;
            char g=stack_pop(st);
            if(g!='[')
            {
                return 0;
            }
        }
    }
    if(a!=b)
    {
        return 0;
    }

    return 1;
}
    /* TODO:
       1. Stack *st = stack_create();
       2. 遍历字符串 s：
            如果是 '(' '[' '{'  → stack_push
            如果是 ')' '] '}'  → 
                如果栈空 → 不匹配
                取出栈顶，看是不是对应的左括号
                    是 → stack_pop
                    不是 → 不匹配
       3. 扫完后：栈必须为空才匹配
       4. 别忘了 stack_free
       ⚠️ 配对关系：')' 对 '('，']' 对 '['，'}' 对 '{'
       ⚠️ 可以用一个辅助函数帮你判断，也可以直接在循环里 switch
    */
   
