#ifndef DA_H
#define DA_H

/* ============================================================
 *  动态数组（Dynamic Array）—— 函数声明
 *
 *  这就是 C++ vector / Java ArrayList 的原理：
 *      底层是一块 malloc 出来的数组，
 *      装满了就 realloc 一块更大的，把数据搬过去。
 *
 *  本项目采用【容量翻倍】策略：初始 4，满了变 8，再满变 16 ...
 * ============================================================ */

#define DA_INIT_CAPACITY 4      /* 初始容量 */

typedef struct {
    int *data;          /* 指向 malloc 出来的数组 */
    int  size;          /* 当前有几个元素 */
    int  capacity;      /* 当前能装几个（>= size） */
} DynArray;

/* 1. 创建：malloc 结构体 + malloc 初始数组
       size = 0, capacity = DA_INIT_CAPACITY */
DynArray *da_create(void);

/* 2. 尾部添加一个元素
       ⚠️ 如果 size == capacity，先扩容（容量翻倍）再添加 */
void da_push(DynArray *a, int val);

/* 3. 取【下标 i】的元素
       ⚠️ C 语言的数组下标从 0 开始：下标 0 就是【第 1 个】元素
       ⚠️ 越界返回 -1（本项目约定数据都是非负数） */
int da_get(DynArray *a, int i);

/* 4. 修改【下标 i】的元素
       越界返回 0，成功返回 1 */
int da_set(DynArray *a, int i, int val);

/* 5. 当前元素个数 */
int da_size(DynArray *a);

/* 6. 当前容量 */
int da_capacity(DynArray *a);

/* 7. 打印：格式 [1, 2, 3] */
void da_print(DynArray *a);

/* 8. 释放：先 free(data)，再 free(结构体本身) */
void da_free(DynArray *a);

#endif  /* DA_H */
