#include <stdio.h>
#include <stdlib.h>
#include "da.h"

/* ============================================================
 *  测试程序 —— 不要动这个文件！
 * ============================================================ */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *msg, int line)
{
    if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }
    else      { g_fail++; printf("  [FAIL] %s        <-- test.c 第 %d 行\n", msg, line); }
}
#define CHECK(cond, msg) check((cond), (msg), __LINE__)

int main(void)
{
    printf("\n");
    printf("##################################################\n");
    printf("#   项目 3：动态数组 —— 自动测试                 #\n");
    printf("##################################################\n");

    /* ---------------- 0. 前置检查 ---------------- */
    printf("\n---------- 0. 前置检查：da_create ----------\n");
    {
        DynArray *probe = da_create();
        if (probe == NULL)
        {
            printf("\n  !! 【前置检查失败】da_create 返回了 NULL\n");
            printf("  请先实现 da_create，再跑测试。\n\n");
            printf("==================================================\n");
            printf("  测试结果：  PASS = 0   FAIL = 1\n");
            printf("==================================================\n\n");
            return 0;
        }
        /* ★ 直接看结构体里的字段，不通过 da_size / da_capacity
           —— 避免"检查 da_create 却依赖别的函数"这种耦合 */
        CHECK(probe->data != NULL,                    "新建：data 已经分配");
        CHECK(probe->size == 0,                       "新建：size = 0");
        CHECK(probe->capacity == DA_INIT_CAPACITY,    "新建：capacity = 4");
        free(probe->data);                            /* 直接释放，不依赖 da_free */
        free(probe);
    }

    /* ---------------- 1. da_push ---------------- */
    printf("\n---------- 1. da_push ----------\n");
    {
        /* ★ 全部直接看结构体字段，不经过 da_size / da_get
           —— 这样"只实现了 da_push"也能得到准确反馈 */
        DynArray *a = da_create();

        da_push(a, 10);
        CHECK(a->size == 1,       "push 1 个 -> size = 1");
        CHECK(a->data[0] == 10,   "第 1 个元素（下标 0）= 10");

        da_push(a, 20);
        da_push(a, 30);
        CHECK(a->size == 3,       "push 3 个 -> size = 3");
        CHECK(a->data[1] == 20 && a->data[2] == 30, "第 1、2 个元素正确");
        CHECK(a->capacity == 4,   "3 个元素时还没扩容，capacity 仍然是 4");

        da_free(a);
    }

    /* ---------------- 2. 扩容轨迹（本项目核心）---------------- */
    printf("\n---------- 2. 扩容轨迹（核心）----------\n");
    {
        DynArray *a = da_create();
        int caps[32], capn = 0;

        caps[capn++] = a->capacity;           /* 初始 4 */

        for (int i = 1; i <= 20; i++) {
            da_push(a, i);
            int c = a->capacity;
            if (c != caps[capn - 1]) caps[capn++] = c;
        }

        printf("  容量变化序列: ");
        for (int i = 0; i < capn; i++) printf("%d ", caps[i]);
        printf("\n");

        CHECK(capn == 4, "容量只变了 3 次");
        CHECK(caps[0] == 4 && caps[1] == 8 && caps[2] == 16 && caps[3] == 32,
              "扩容轨迹：4 -> 8 -> 16 -> 32");

        CHECK(a->size == 20, "20 个元素都记上了");

        /* 容量足够大才敢直接访问，避免越界读 */
        if (a->size == 20 && a->capacity >= 20) {
            int ok = 1;
            for (int i = 0; i < 20; i++)
                if (a->data[i] != i + 1) { ok = 0; break; }
            CHECK(ok, "扩容后原有数据没有丢失");
        } else {
            CHECK(0, "扩容后原有数据没有丢失（size 或 capacity 不对，跳过越界访问）");
        }

        da_free(a);
    }

    /* ---------------- 3. 大数据量 ---------------- */
    printf("\n---------- 3. 大数据量（10000 个）----------\n");
    {
        DynArray *a = da_create();
        int N = 10000;

        for (int i = 0; i < N; i++) da_push(a, i * 2);

        CHECK(a->size == N, "size = 10000");

        if (a->size == N && a->capacity >= N) {
            CHECK(a->data[0] == 0,        "第 1 个元素（下标 0）= 0");
            CHECK(a->data[5000] == 10000, "第 5000 个 = 10000");
            CHECK(a->data[9999] == 19998, "最后一个 = 19998");

            int ok = 1;
            for (int i = 0; i < N; i++)
                if (a->data[i] != i * 2) { ok = 0; break; }
            CHECK(ok, "全部 10000 个元素都正确");
        } else {
            CHECK(0, "第 1 个元素（下标 0）= 0（size 或 capacity 不对，跳过越界访问）");
            CHECK(0, "第 5000 个 = 10000");
            CHECK(0, "最后一个 = 19998");
            CHECK(0, "全部 10000 个元素都正确");
        }

        da_free(a);
    }

    /* ---------------- 4. da_get 越界 ---------------- */
    printf("\n---------- 4. da_get 越界保护 ----------\n");
    {
        DynArray *a = da_create();
        da_push(a, 1);
        da_push(a, 2);

        CHECK(da_get(a, 0) == 1,   "取下标 0（就是第 1 个）");
        CHECK(da_get(a, 1) == 2,   "取下标 1（第 2 个）");
        CHECK(da_get(a, 2) == -1,  "取下标 2（第 3 个，越界）-> -1");
        CHECK(da_get(a, -1) == -1, "取下标 -1（负数，越界）-> -1");
        CHECK(da_get(a, 99) == -1, "取下标 99（越界）-> -1");

        da_free(a);
    }

    /* ---------------- 5. da_set ---------------- */
    printf("\n---------- 5. da_set ----------\n");
    {
        DynArray *a = da_create();
        da_push(a, 1);
        da_push(a, 2);
        da_push(a, 3);

        CHECK(da_set(a, 1, 99) == 1, "设置下标 1（第 2 个）-> 返回 1");
        CHECK(da_get(a, 1) == 99,    "下标 1 变成了 99");
        CHECK(da_get(a, 0) == 1 && da_get(a, 2) == 3, "其它元素没受影响");

        CHECK(da_set(a, 3, 0) == 0,  "设置下标 3（越界）-> 返回 0");
        CHECK(da_set(a, -1, 0) == 0, "设置下标 -1（越界）-> 返回 0");

        da_free(a);
    }

    /* ---------------- 6. da_print（人眼检查）---------------- */
    printf("\n---------- 6. da_print（请人眼检查下面这行）----------\n");
    {
        DynArray *a = da_create();
        da_push(a, 10);
        da_push(a, 20);
        da_push(a, 30);
        printf("  实际输出：");
        da_print(a);
        printf("  期望输出：[10, 20, 30]\n");

        DynArray *e = da_create();
        printf("  空数组输出：");
        da_print(e);
        printf("  期望输出：[]\n");

        da_free(a);
        da_free(e);
    }

    /* ---------------- 7. da_free ---------------- */
    printf("\n---------- 7. da_free ----------\n");
    {
        DynArray *a = da_create();
        for (int i = 0; i < 100; i++) da_push(a, i);
        da_free(a);
        printf("  (释放 100 个元素的数组，没有崩溃)\n");

        da_free(NULL);
        printf("  (da_free(NULL) 也没崩)\n");
    }

    /* ---------------- 总结 ---------------- */
    printf("\n==================================================\n");
    printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail);
    if (g_fail == 0)
        printf("  🎉 全部通过！项目 3 完成！\n");
    else
        printf("  还有 %d 个没通过，继续加油\n", g_fail);
    printf("==================================================\n\n");

    return 0;
}
