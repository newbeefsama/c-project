#include <stdlib.h>
#include <stdio.h>
#include "da.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  实现 8 个函数。
 *  核心是 da_push 里的【扩容】——那是这个项目的重点。
 * ============================================================ */


/* ---------- 1. 创建 ---------- */
DynArray *da_create(void)
{
    DynArray* a=malloc(sizeof(DynArray));
    if(a==NULL)
    {
        return NULL;
    }
    a->data=malloc(sizeof(int)*DA_INIT_CAPACITY);
    if(a->data==NULL)
    {
        free(a);
        return NULL;
    }
    a->size=0;
    a->capacity=DA_INIT_CAPACITY;
    return a;
    /* TODO:
       1. malloc 一个 DynArray
       2. malloc 一块 DA_INIT_CAPACITY 个 int 的数组，赋给 a->data
       3. a->size = 0;  a->capacity = DA_INIT_CAPACITY;
       4. 返回它
       ⚠️ 两次 malloc 都要判空
    */
    }


/* ---------- 2. 尾部添加（含扩容）★ 核心 ---------- */
void da_push(DynArray *a, int val)
{
    if(a->size==a->capacity)
    {
        int newcup=a->capacity*2;
        int* tmp=realloc(a->data,newcup*sizeof(int));
        if(tmp==NULL)
        {
            return ;
        }
        a->data=tmp;
        a->capacity=newcup;
    }
        a->data[a->size]=val;
        a->size++;
    /* TODO:
       第 1 步：检查是不是满了
           if (a->size == a->capacity) { ...扩容... }

       第 2 步：扩容（三步）
           ① 新容量 = 旧容量 * 2
           ② int *newData = realloc(a->data, 新容量 * sizeof(int));
              ⚠️ realloc 可能失败！失败了不能把 a->data 弄丢：
                 int *tmp = realloc(...);
                 if (tmp == NULL) return;
                 a->data = tmp;
           ③ a->capacity = 新容量;

       第 3 步：把元素放进去
           a->data[a->size] = val;
           a->size++;
       ⚠️ 扩容要放在"写入"之前
    */
}


/* ---------- 3. 取值 ---------- */
int da_get(DynArray *a, int i)
{
    if(i<0||i>=a->size)
    {
        return -1;
    }
    return a->data[i];
    /* TODO:
       if (i < 0 || i >= a->size) return -1;     ← 越界保护
       return a->data[i];
       ⚠️ 判断用的是 size（有几个元素），不是 capacity（能装几个）
    */
}


/* ---------- 4. 改值 ---------- */
int da_set(DynArray *a, int i, int val)
{
    if(i>=a->size||i<0)
    {
        return 0;
    }
    else{
        a->data[i]=val;
        return 1;
    }
    /* TODO:
       越界返回 0，成功改完返回 1
    */
}


/* ---------- 5. 元素个数 ---------- */
int da_size(DynArray *a)
{
    return a->size;
}


/* ---------- 6. 当前容量 ---------- */
int da_capacity(DynArray *a)
{
    return a->capacity;
}


/* ---------- 7. 打印 ---------- */
void da_print(DynArray *a)
{
    (void)a;
    printf("[");
    for(int i=0;i<a->size;i++)
    {
        printf("%d",a->data[i]);
        if(i<(a->size-1))
        {
            printf(", ");
        }
    }
    printf("]");
    printf("\n");        /* 占位：避免"参数未使用"警告；你写完后删掉 */
    /* TODO: 格式 [1, 2, 3]
       提示：先打印 '['，然后循环打印元素和 ", "，最后打印 ']' 和换行
    */
}


/* ---------- 8. 释放 ---------- */
void da_free(DynArray *a)
{
    if(a!=NULL){
    free(a->data);
        free(a);}
        /* 占位：避免"参数未使用"警告；你写完后删掉 */
    /* TODO:
       1. free(a->data);      ← 先释放内部的数组
       2. free(a);            ← 再释放结构体本身
       ⚠️ 顺序不能反！先 free(a) 的话就找不到 a->data 了（内存泄漏）
       ⚠️ 要能处理 a == NULL 的情况
    */
}
