#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* ============================================================
 *  测试程序（哨兵版）—— 不要动这个文件！
 *
 *  ⚠️ 和上一版测试的区别：所有函数都传 head 本身，不再传 &head
 * ============================================================ */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *msg, int line)
{
    if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }
    else      { g_fail++; printf("  [FAIL] %s        <-- test.c 第 %d 行\n", msg, line); }
}
#define CHECK(cond, msg) check((cond), (msg), __LINE__)

/* 从 head->next 开始数数据节点（带安全上限） */
static int count_nodes(Node *head)
{
    int n = 0;
    if (head == NULL) return 0;
    for (Node *p = head->next; p != NULL && n < 10000; p = p->next) n++;
    return n;
}

/* 检查 head->next 之后的内容是否和数组一样 */
static int list_eq(Node *head, const int *want, int n)
{
    int i = 0;
    if (head == NULL) return 0;
    for (Node *p = head->next; p != NULL && i < 10000; p = p->next) {
        if (i >= n || p->data != want[i]) return 0;
        i++;
    }
    return i == n;
}

/* 用尾插造一条链表，返回哨兵 */
static Node *build(const int *vals, int n)
{
    Node *h = list_create();
    for (int i = 0; i < n; i++) list_push_back(h, vals[i]);
    return h;
}

int main(void)
{
    printf("\n");
    printf("##################################################\n");
    printf("#   项目 2.5：单链表（哨兵版）—— 自动测试        #\n");
    printf("##################################################\n");

    /* ---------------- 0. 前置检查 ---------------- */
    printf("\n---------- 0. 前置检查：list_create ----------\n");
    {
        Node *probe = list_create();
        if (probe == NULL)
        {
            printf("\n  !! 【前置检查失败】list_create 返回了 NULL\n\n");
            printf("  哨兵版和上一版最大的区别就在这里：\n");
            printf("    · 上一版：空链表 = NULL\n");
            printf("    · 哨兵版：空链表 = 一个真实的哨兵节点（它的 next 是 NULL）\n\n");
            printf("  请先实现 list_create，再跑测试。\n\n");
            printf("==================================================\n");
            printf("  测试结果：  PASS = 0   FAIL = 1\n");
            printf("  还有 1 个没通过，继续加油\n");
            printf("==================================================\n\n");
            return 0;
        }
        CHECK(probe->next == NULL, "list_create：哨兵的 next 初始化为 NULL");
        free(probe);        /* 这里直接 free，不依赖 list_free */
    }

    /* ---------------- 1. list_push_front ---------------- */
    printf("\n---------- 1. list_push_front（头插）----------\n");
    {
        Node *h = list_create();
        int a1[] = {10}, a2[] = {20, 10}, a3[] = {30, 20, 10};

        list_push_front(h, 10);
        CHECK(list_eq(h, a1, 1), "空链表头插 10");

        list_push_front(h, 20);
        CHECK(list_eq(h, a2, 2), "再头插 20  ->  20 10");

        list_push_front(h, 30);
        CHECK(list_eq(h, a3, 3), "再头插 30  ->  30 20 10");

        CHECK(h != NULL, "哨兵本身没有被动过");

        list_free(&h);
    }

    /* ---------------- 2. list_push_back ---------------- */
    printf("\n---------- 2. list_push_back（尾插）----------\n");
    {
        Node *h = list_create();
        int b1[] = {10}, b3[] = {10, 20, 30};

        list_push_back(h, 10);
        CHECK(list_eq(h, b1, 1), "空链表尾插 10（不用特判空链表！）");

        list_push_back(h, 20);
        list_push_back(h, 30);
        CHECK(list_eq(h, b3, 3), "继续尾插  ->  10 20 30");

        list_free(&h);
    }

    /* ---------------- 3. list_size ---------------- */
    printf("\n---------- 3. list_size（数据节点个数，不算哨兵）----------\n");
    {
        int v[] = {1, 2, 3, 4, 5};
        Node *h = list_create();

        CHECK(list_size(h) == 0, "空链表 size = 0（哨兵不算）");

        list_push_back(h, 1);
        CHECK(list_size(h) == 1, "1 个数据节点 size = 1");

        list_free(&h);

        h = build(v, 5);
        CHECK(list_size(h) == 5, "5 个数据节点 size = 5");

        list_free(&h);
    }

    /* ---------------- 4. list_find ---------------- */
    printf("\n---------- 4. list_find（查找）----------\n");
    {
        int v[] = {10, 20, 30, 20};
        Node *h = build(v, 4);

        CHECK(list_find(h, 10) == h->next, "找第一个数据节点");
        CHECK(list_find(h, 30) != NULL &&
              list_find(h, 30)->data == 30, "找中间的 30");
        CHECK(list_find(h, 99) == NULL,   "找不到 -> NULL");
        CHECK(list_find(h, 10) != h,      "不会返回哨兵本身");

        list_free(&h);
    }

    /* ---------------- 5. list_remove ---------------- */
    printf("\n---------- 5. list_remove（删除，不用特判头节点！）----------\n");
    {
        int v[] = {10, 20, 30};
        Node *h = build(v, 3);
        int r1[] = {10, 30}, r2[] = {30};

        CHECK(list_remove(h, 20) == 1,   "删除中间节点 20");
        CHECK(list_eq(h, r1, 2),         "剩下 10 30");

        CHECK(list_remove(h, 10) == 1,   "删除第一个数据节点 10（不用特判！）");
        CHECK(list_eq(h, r2, 1),         "剩下 30");

        CHECK(list_remove(h, 99) == 0,   "删除不存在的 99 -> 0");
        CHECK(list_eq(h, r2, 1),         "删除失败时链表不变");

        CHECK(list_remove(h, 30) == 1,   "删除最后一个节点");
        CHECK(list_size(h) == 0,         "链表变空（只剩哨兵）");
        CHECK(h != NULL,                 "哨兵还在");

        CHECK(list_remove(h, 1) == 0,    "空链表删除 -> 0");

        list_free(&h);
    }

    /* ---------------- 6. list_reverse ---------------- */
    printf("\n---------- 6. list_reverse（反转）----------\n");
    {
        Node *h = list_create();

        list_reverse(h);
        CHECK(h->next == NULL, "空链表反转后还是空");

        list_push_back(h, 1);
        list_reverse(h);
        { int v[] = {1}; CHECK(list_eq(h, v, 1), "单节点反转不变"); }

        list_push_back(h, 2);
        list_push_back(h, 3);
        { int v[] = {1, 2, 3}; CHECK(list_eq(h, v, 3), "反转前是 1 2 3"); }

        list_reverse(h);
        { int v[] = {3, 2, 1}; CHECK(list_eq(h, v, 3), "反转后变成 3 2 1"); }

        list_reverse(h);
        { int v[] = {1, 2, 3}; CHECK(list_eq(h, v, 3), "再反转回来 1 2 3"); }

        list_free(&h);
    }

    /* ---------------- 7. list_free ---------------- */
    printf("\n---------- 7. list_free（释放，含哨兵）----------\n");
    {
        int v[] = {1, 2, 3, 4, 5};
        Node *h = build(v, 5);

        CHECK(count_nodes(h) == 5, "释放前有 5 个数据节点");
        list_free(&h);
        CHECK(h == NULL,           "free 之后 head = NULL");

        list_free(&h);
        CHECK(h == NULL,           "再 free 一次也不崩");
    }

    /* ---------------- 8. list_print（人眼检查）---------------- */
    printf("\n---------- 8. list_print（请人眼检查下面这行）----------\n");
    {
        int v[] = {10, 20, 30};
        Node *h = build(v, 3);
        printf("  实际输出：");
        list_print(h);
        printf("  期望输出：10 -> 20 -> 30 -> NULL（不能有哨兵的值）\n");
        list_free(&h);
    }

    /* ---------------- 总结 ---------------- */
    printf("\n==================================================\n");
    printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail);
    if (g_fail == 0)
        printf("  🎉 全部通过！哨兵版完成！\n");
    else
        printf("  还有 %d 个没通过，继续加油\n", g_fail);
    printf("==================================================\n\n");

    return 0;
}
