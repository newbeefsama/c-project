#include <stdio.h>
#include <stdlib.h>
#include "ds.h"

/* ============================================================
 *  测试程序 —— 不要动这个文件！
 *
 *  ★ 设计原则：每个函数的测试【只依赖它自己】+ 直接看结构体字段，
 *    不通过别的函数来检查，避免"函数没写导致误报"。
 * ============================================================ */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *msg, int line)
{
    if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }
    else      { g_fail++; printf("  [FAIL] %s        <-- test.c 第 %d 行\n", msg, line); }
}
#define CHECK(cond, msg) check((cond), (msg), __LINE__)

/* 直接看栈的内部数组（不经过 stack_top / stack_pop） */
static int stack_eq(Stack *s, const int *want, int n)
{
    if (s == NULL || s->size != n) return 0;
    for (int i = 0; i < n; i++)
        if (s->data[i] != want[i]) return 0;
    return 1;
}

/* 直接数队列的节点（不经过 queue_size） */
static int queue_count(Queue *q)
{
    int n = 0;
    if (q == NULL) return -1;
    for (QNode *p = q->front; p != NULL && n < 10000; p = p->next) n++;
    return n;
}

/* 直接从 front 开始遍历队列（不经过 queue_front / queue_pop） */
static int queue_eq(Queue *q, const int *want, int n)
{
    int i = 0;
    if (q == NULL) return 0;
    for (QNode *p = q->front; p != NULL && i < 10000; p = p->next) {
        if (i >= n || p->data != want[i]) return 0;
        i++;
    }
    return i == n;
}

int main(void)
{
    printf("\n");
    printf("##################################################\n");
    printf("#   项目 4：栈 和 队列 —— 自动测试               #\n");
    printf("##################################################\n");

    /* ================= 前置检查 ================= */
    printf("\n---------- 0. 前置检查 ----------\n");
    {
        Stack *s = stack_create();
        if (s == NULL)
        {
            printf("\n  !! stack_create 返回了 NULL，请先实现它。\n\n");
            printf("==================================================\n");
            printf("  测试结果：  PASS = 0   FAIL = 1\n");
            printf("==================================================\n\n");
            return 0;
        }
        CHECK(s->data != NULL,                 "stack_create：内部数组已分配");
        CHECK(s->size == 0,                    "stack_create：size = 0");
        CHECK(s->capacity == 4,                "stack_create：capacity = 4");
        free(s->data); free(s);

        Queue *q = queue_create();
        if (q == NULL)
        {
            printf("\n  !! queue_create 返回了 NULL，请先实现它。\n\n");
            printf("==================================================\n");
            printf("  测试结果：  PASS = %d   FAIL = 1\n", g_pass);
            printf("==================================================\n\n");
            return 0;
        }
        CHECK(q->front == NULL,                "queue_create：front = NULL");
        CHECK(q->rear == NULL,                 "queue_create：rear = NULL");
        CHECK(q->size == 0,                    "queue_create：size = 0");
        free(q);
    }

    /* ================= Part A：栈 ================= */
    printf("\n==================================================\n");
    printf("  Part A：栈（动态数组实现，LIFO）\n");
    printf("==================================================\n");

    /* ---------------- A1. stack_push ---------------- */
    printf("\n---------- A1. stack_push（入栈）----------\n");
    {
        Stack *s = stack_create();
        int v1[] = {10}, v2[] = {10, 20}, v3[] = {10, 20, 30};

        stack_push(s, 10);
        CHECK(stack_eq(s, v1, 1), "入栈 10  ->  栈内 [10]");

        stack_push(s, 20);
        CHECK(stack_eq(s, v2, 2), "入栈 20  ->  栈内 [10, 20]（20 在栈顶）");

        stack_push(s, 30);
        CHECK(stack_eq(s, v3, 3), "入栈 30  ->  栈内 [10, 20, 30]");
        CHECK(s->size == 3,       "size = 3");
        CHECK(s->capacity == 4,   "还没扩容，capacity 仍然是 4");

        stack_free(s);
    }

    /* ---------------- A2. 栈的扩容 ---------------- */
    printf("\n---------- A2. 栈的扩容（复用项目 3 的逻辑）----------\n");
    {
        Stack *s = stack_create();
        int caps[32], capn = 0;
        caps[capn++] = s->capacity;

        for (int i = 1; i <= 20; i++) {
            stack_push(s, i);
            if (s->capacity != caps[capn - 1]) caps[capn++] = s->capacity;
        }

        printf("  容量变化序列: ");
        for (int i = 0; i < capn; i++) printf("%d ", caps[i]);
        printf("\n");

        CHECK(capn == 4, "容量变了 3 次");
        CHECK(caps[0] == 4 && caps[1] == 8 && caps[2] == 16 && caps[3] == 32,
              "扩容轨迹：4 -> 8 -> 16 -> 32");
        CHECK(s->size == 20, "20 个元素都在");

        if (s->size == 20 && s->capacity >= 20) {
            int ok = 1;
            for (int i = 0; i < 20; i++)
                if (s->data[i] != i + 1) { ok = 0; break; }
            CHECK(ok, "扩容后数据没丢");
        } else {
            CHECK(0, "扩容后数据没丢（size/capacity 不对，跳过越界访问）");
        }

        stack_free(s);
    }

    /* ---------------- A3. stack_pop ---------------- */
    printf("\n---------- A3. stack_pop（出栈，LIFO 后进先出）----------\n");
    {
        Stack *s = stack_create();
        int v2[] = {10, 20};

        stack_push(s, 10);
        stack_push(s, 20);
        stack_push(s, 30);

        CHECK(stack_pop(s) == 30, "第 1 次 pop -> 30（最后进的先出）");
        CHECK(s->size == 2,       "size 变成 2");
        CHECK(stack_eq(s, v2, 2), "栈内还剩 [10, 20]");

        CHECK(stack_pop(s) == 20, "第 2 次 pop -> 20");
        CHECK(stack_pop(s) == 10, "第 3 次 pop -> 10");
        CHECK(s->size == 0,       "size 变成 0");
        CHECK(stack_pop(s) == -1, "对空栈 pop -> -1（不能崩）");
        CHECK(stack_pop(s) == -1, "再 pop 一次还是 -1");

        stack_free(s);
    }

    /* ---------------- A4. stack_top ---------------- */
    printf("\n---------- A4. stack_top（看栈顶，不拿走）----------\n");
    {
        Stack *s = stack_create();

        CHECK(stack_top(s) == -1, "空栈 top -> -1");

        stack_push(s, 10);
        stack_push(s, 20);
        CHECK(stack_top(s) == 20, "top -> 20");
        CHECK(s->size == 2,       "top 之后 size 不变（没拿走）");
        CHECK(stack_top(s) == 20, "再 top 还是 20");

        stack_free(s);
    }

    /* ---------------- A5-A6. stack_empty / stack_size ---------------- */
    printf("\n---------- A5. stack_empty / stack_size ----------\n");
    {
        Stack *s = stack_create();

        CHECK(stack_empty(s) == 1, "空栈 empty -> 1");
        CHECK(stack_size(s) == 0,  "空栈 size -> 0");

        stack_push(s, 1);
        CHECK(stack_empty(s) == 0, "非空 empty -> 0");
        CHECK(stack_size(s) == 1,  "size -> 1");

        stack_push(s, 2);
        stack_push(s, 3);
        CHECK(stack_size(s) == 3,  "size -> 3");

        stack_pop(s);
        CHECK(stack_size(s) == 2,  "pop 一次后 size -> 2");

        stack_free(s);
    }

    /* ================= Part B：队列 ================= */
    printf("\n==================================================\n");
    printf("  Part B：队列（链表实现，FIFO）\n");
    printf("==================================================\n");

    /* ---------------- B1. queue_push ---------------- */
    printf("\n---------- B1. queue_push（入队，加到队尾）----------\n");
    {
        Queue *q = queue_create();
        int v1[] = {10}, v3[] = {10, 20, 30};

        queue_push(q, 10);
        CHECK(queue_eq(q, v1, 1), "入队 10");
        CHECK(q->front == q->rear, "队列里只有一个节点时 front == rear");

        queue_push(q, 20);
        queue_push(q, 30);
        CHECK(queue_eq(q, v3, 3), "入队 20 30  ->  10 -> 20 -> 30");
        CHECK(q->front != NULL && q->front->data == 10, "front 还是 10（队头不动）");
        CHECK(q->rear != NULL && q->rear->data == 30,   "rear 变成了 30（队尾更新）");

        queue_free(q);
    }

    /* ---------------- B2. queue_pop ---------------- */
    printf("\n---------- B2. queue_pop（出队，FIFO 先进先出）----------\n");
    {
        Queue *q = queue_create();
        int v2[] = {20, 30};

        for (int i = 1; i <= 3; i++) queue_push(q, i * 10);

        CHECK(queue_pop(q) == 10, "第 1 次 pop -> 10（先进的先出）");
        CHECK(queue_eq(q, v2, 2), "队列还剩 20 -> 30");
        CHECK(q->front != NULL && q->front->data == 20, "front 前移到了 20");

        CHECK(queue_pop(q) == 20, "第 2 次 pop -> 20");
        CHECK(queue_pop(q) == 30, "第 3 次 pop -> 30");
        CHECK(queue_count(q) == 0, "队列空了");
        CHECK(queue_pop(q) == -1,  "对空队列 pop -> -1");
        CHECK(q->rear == NULL,     "队列空了之后 rear 要变成 NULL");

        queue_free(q);
    }

    /* ---------------- B3. ⭐ 出队到空之后再入队 ---------------- */
    printf("\n---------- B3. 出队到空之后再入队（最容易错的地方）----------\n");
    {
        Queue *q = queue_create();
        int v[] = {99};

        queue_push(q, 1);
        queue_pop(q);                 /* 出队，此时队列变空 */
        CHECK(q->rear == NULL,        "出队到空：rear 被清成 NULL");
        CHECK(q->front == NULL,       "出队到空：front 也是 NULL");

        queue_push(q, 99);            /* 再从空队列入队 */
        CHECK(queue_eq(q, v, 1),      "空队列再入队 -> [99]");
        CHECK(q->front == q->rear,    "front 和 rear 都指向新节点");
        CHECK(queue_pop(q) == 99,     "能正常取出 99");

        /* 再来一轮，确认没有累积问题 */
        queue_push(q, 7);
        CHECK(queue_pop(q) == 7,      "再入队 7 再出队 -> 7");
        CHECK(queue_empty(q) == 1,    "又空了");

        queue_free(q);
    }

    /* ---------------- B4. queue_front ---------------- */
    printf("\n---------- B4. queue_front（看队头，不拿走）----------\n");
    {
        Queue *q = queue_create();

        CHECK(queue_front(q) == -1, "空队列 front -> -1");

        queue_push(q, 10);
        queue_push(q, 20);
        CHECK(queue_front(q) == 10, "front -> 10");
        CHECK(queue_count(q) == 2,  "front 之后元素个数不变");

        queue_free(q);
    }

    /* ---------------- B5-B6. queue_empty / queue_size ---------------- */
    printf("\n---------- B5. queue_empty / queue_size ----------\n");
    {
        Queue *q = queue_create();

        CHECK(queue_empty(q) == 1, "空队列 empty -> 1");
        CHECK(queue_size(q) == 0,  "空队列 size -> 0");

        queue_push(q, 1);
        queue_push(q, 2);
        queue_push(q, 3);
        CHECK(queue_empty(q) == 0, "非空 empty -> 0");
        CHECK(queue_size(q) == 3,  "size -> 3");

        queue_pop(q);
        CHECK(queue_size(q) == 2,  "pop 一次后 size -> 2");

        queue_pop(q);
        queue_pop(q);
        CHECK(queue_size(q) == 0,  "全出队后 size -> 0");
        CHECK(queue_empty(q) == 1, "又空了");

        queue_free(q);
    }

    /* ---------------- B7. 大数据量（验证没有内存问题）---------------- */
    printf("\n---------- B7. 10000 个元素进出 ----------\n");
    {
        Queue *q = queue_create();
        int N = 10000;

        for (int i = 0; i < N; i++) queue_push(q, i);
        CHECK(queue_size(q) == N, "入队 10000 个 -> size = 10000");

        int ok = 1;
        for (int i = 0; i < N; i++) {
            if (queue_pop(q) != i) { ok = 0; break; }
        }
        CHECK(ok, "出队顺序完全正确（0,1,2,...,9999）");
        CHECK(queue_empty(q) == 1, "全部出队后为空");

        queue_free(q);
    }

    /* ================= Part C：括号匹配 ================= */
    printf("\n==================================================\n");
    printf("  Part C：括号匹配（用栈解决）\n");
    printf("==================================================\n");

    printf("\n---------- C1. 应该匹配的 ----------\n");
    CHECK(brackets_match("") == 1,             "空字符串 -> 1");
    CHECK(brackets_match("()") == 1,           "\"()\" -> 1");
    CHECK(brackets_match("(a+b)") == 1,        "\"(a+b)\" -> 1");
    CHECK(brackets_match("{[()]}") == 1,       "\"{[()]}\" -> 1");
    CHECK(brackets_match("a[b]{c}(d)") == 1,   "混合内容 -> 1");
    CHECK(brackets_match("((()))") == 1,       "\"((()))\" -> 1");
    CHECK(brackets_match("abc") == 1,          "没有括号 -> 1");

    printf("\n---------- C2. 不应该匹配的 ----------\n");
    CHECK(brackets_match("(") == 0,            "\"(\" -> 0（左括号多了）");
    CHECK(brackets_match(")") == 0,            "\")\" -> 0（右括号多了）");
    CHECK(brackets_match("(((") == 0,          "\"(((\" -> 0");
    CHECK(brackets_match(")))") == 0,          "\")))\" -> 0");
    CHECK(brackets_match("([)]") == 0,         "\"([)]\" -> 0（交叉了）");
    CHECK(brackets_match("(]") == 0,           "\"(]\" -> 0（类型不对）");
    CHECK(brackets_match("{") == 0,            "\"{\" -> 0");
    CHECK(brackets_match("(a[b)") == 0,        "嵌错位置 -> 0");

    /* ================= 总结 ================= */
    printf("\n==================================================\n");
    printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail);
    if (g_fail == 0)
        printf("  🎉 全部通过！项目 4 完成！\n");
    else
        printf("  还有 %d 个没通过，继续加油\n", g_fail);
    printf("==================================================\n\n");

    return 0;
}
