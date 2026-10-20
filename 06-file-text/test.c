#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "text.h"

/* ============================================================
 *  测试程序 —— 不要动这个文件！
 *
 *  ★ 每个函数的测试只依赖它自己 + 直接看结构体字段
 * ============================================================ */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *msg, int line)
{
    if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }
    else      { g_fail++; printf("  [FAIL] %s        <-- test.c 第 %d 行\n", msg, line); }
}
#define CHECK(cond, msg) check((cond), (msg), __LINE__)

/* 写一个测试用的文件 */
static void make_file(const char *path, const char *content)
{
    FILE *fp = fopen(path, "w");
    if (fp != NULL) { fputs(content, fp); fclose(fp); }
}

/* 在 wl 里找 word 的 count，找不到返回 -1 */
static int wl_get(WordList *wl, const char *word)
{
    if (wl == NULL || wl->items == NULL) return -1;
    for (int i = 0; i < wl->size && i < 10000; i++)
        if (strcmp(wl->items[i].word, word) == 0) return wl->items[i].count;
    return -1;
}

static int wl_well_formed(WordList *wl)
{
    if (wl == NULL || wl->items == NULL) return 0;
    if (wl->size < 0 || wl->size > wl->capacity) return 0;
    for (int i = 0; i < wl->size; i++) {
        if (wl->items[i].word[0] == '\0') return 0;
        if (wl->items[i].count <= 0) return 0;
    }
    return 1;
}

int main(void)
{
    printf("\n");
    printf("##################################################\n");
    printf("#   项目 5：文本统计（文件操作）—— 自动测试      #\n");
    printf("##################################################\n");

    /* ================= Part A：文件基础 ================= */
    printf("\n==================================================\n");
    printf("  Part A：文件基础\n");
    printf("==================================================\n");

    /* ---------------- A1. file_write_all（独立测试，用标准库读回来验证）---------------- */
    printf("\n---------- A1. file_write_all（写文件）----------\n");
    {
        const char *path = "_t_w.txt";
        char raw[256];
        long rd;

        CHECK(file_write_all(path, "hello world") == 1, "写入 \"hello world\" 返回 1");

        /* ★ 用【标准库】读回来验证，不依赖 file_read_all */
        rd = -1;
        {
            FILE *fp = fopen(path, "rb");
            if (fp != NULL) {
                rd = (long)fread(raw, 1, sizeof(raw) - 1, fp);
                fclose(fp);
                if (rd >= 0) raw[rd] = '\0';
            }
        }
        CHECK(rd == 11 && strcmp(raw, "hello world") == 0, "文件里确实是 \"hello world\"");

        CHECK(file_write_all(path, "") == 1, "写入空字符串返回 1");
        rd = -1;
        {
            FILE *fp = fopen(path, "rb");
            if (fp != NULL) {
                rd = (long)fread(raw, 1, sizeof(raw) - 1, fp);
                fclose(fp);
                if (rd >= 0) raw[rd] = '\0';
            }
        }
        CHECK(rd == 0, "空字符串写入后文件是空的");

        remove(path);
    }

    /* ---------------- A2. file_read_all（独立测试，用标准库造文件）---------------- */
    printf("\n---------- A2. file_read_all（读文件）----------\n");
    {
        const char *path = "_t_r.txt";
        int len = -1;
        char *got;

        /* ★ 用【标准库】造文件，不依赖 file_write_all */
        make_file(path, "hello world");
        got = file_read_all(path, &len);
        CHECK(got != NULL, "读回来不是 NULL");
        if (got != NULL) {
            CHECK(strcmp(got, "hello world") == 0, "内容一致");
            CHECK(len == 11, "长度是 11");
            free(got);
        } else {
            CHECK(0, "内容一致（读失败，跳过）");
            CHECK(0, "长度是 11");
        }

        make_file(path, "");
        got = file_read_all(path, &len);
        CHECK(got != NULL && got[0] == '\0', "读空文件 -> 空字符串");
        CHECK(len == 0, "空文件长度 0");
        if (got != NULL) free(got);

        make_file(path, "line1\nline2\nline3");
        got = file_read_all(path, &len);
        CHECK(got != NULL && strcmp(got, "line1\nline2\nline3") == 0, "多行内容一致");
        CHECK(len == 17, "多行长度 17");
        if (got != NULL) free(got);

        /* 超过 64 字节，验证扩容 */
        {
            char big[300];
            for (int i = 0; i < 250; i++) big[i] = (char)('a' + i % 26);
            big[250] = '\0';
            make_file(path, big);
            got = file_read_all(path, &len);
            CHECK(got != NULL && strcmp(got, big) == 0, "250 字符（触发扩容）内容一致");
            CHECK(len == 250, "250 字符长度正确");
            if (got != NULL) free(got);
        }

        remove(path);
    }

    /* ---------------- A3. file_read_all 失败情况 ---------------- */
    printf("\n---------- A3. 读取失败的情况 ----------\n");
    {
        int len = 999;
        char *got = file_read_all("_这个文件不存在_.txt", &len);
        CHECK(got == NULL, "读不存在的文件 -> NULL");

        got = file_read_all("_这个文件不存在_.txt", NULL);
        CHECK(got == NULL, "out_len 传 NULL 也不崩");
    }

    /* ---------------- A4. file_copy ---------------- */
    printf("\n---------- A4. file_copy（复制文件）----------\n");
    {
        const char *src = "_t_src.txt";
        const char *dst = "_t_dst.txt";
        char *a, *b;

        make_file(src, "copy me\nplease\n");
        CHECK(file_copy(src, dst) == 1, "复制成功返回 1");

        a = file_read_all(src, NULL);
        b = file_read_all(dst, NULL);
        CHECK(a != NULL && b != NULL && strcmp(a, b) == 0, "复制后内容相同");
        if (a) free(a);
        if (b) free(b);

        CHECK(file_copy("_不存在_.txt", dst) == 0, "源文件不存在 -> 返回 0");

        remove(src); remove(dst);
    }

    /* ================= Part B：统计 ================= */
    printf("\n==================================================\n");
    printf("  Part B：文件统计\n");
    printf("==================================================\n");

    printf("\n---------- B1. file_stats ----------\n");
    {
        const char *path = "_t_stats.txt";
        FileStats st;

        struct {
            const char *content;
            int lines, words, chars;
            const char *desc;
        } cases[] = {
            { "",               0, 0,  0,  "空文件" },
            { "hello",          1, 1,  5,  "\"hello\"" },
            { "hello world",    1, 2, 11,  "\"hello world\"" },
            { "hello\nworld",   2, 2, 11,  "\"hello\\nworld\"" },
            { "a b\nc d\n",     2, 4,  8,  "\"a b\\nc d\\n\"" },
            { "a b\nc d",       2, 4,  7,  "\"a b\\nc d\"（结尾没换行）" },
            { "  \t \n  ",      2, 0,  7,  "全是空白（7 个字符，结尾不是换行 -> 2 行）" },
        };
        int n = (int)(sizeof(cases) / sizeof(cases[0]));

        for (int i = 0; i < n; i++) {
            char buf[128];
            make_file(path, cases[i].content);
            memset(&st, 0, sizeof(st));

            int ok = file_stats(path, &st);
            snprintf(buf, sizeof(buf), "%s -> lines=%d words=%d chars=%d", cases[i].desc,
                     cases[i].lines, cases[i].words, cases[i].chars);
            CHECK(ok == 1 && st.lines == cases[i].lines &&
                  st.words == cases[i].words && st.chars == cases[i].chars, buf);

            if (ok != 1 || st.lines != cases[i].lines ||
                st.words != cases[i].words || st.chars != cases[i].chars) {
                printf("         实际: lines=%d words=%d chars=%d\n",
                       st.lines, st.words, st.chars);
            }
        }

        CHECK(file_stats("_不存在_.txt", &st) == 0, "文件不存在 -> 返回 0");
        remove(path);
    }

    /* ================= Part C：单词计数 ================= */
    printf("\n==================================================\n");
    printf("  Part C：单词计数\n");
    printf("==================================================\n");

    /* ---------------- C1. wl_create ---------------- */
    printf("\n---------- C1. wl_create ----------\n");
    {
        WordList *wl = wl_create();
        if (wl == NULL) {
            printf("\n  !! wl_create 返回 NULL，Part C 的测试没法继续。\n");
            printf("  （先把 Part C 的 wl_create 实现，或者先看上面 Part A/B 的失败信息）\n\n");
            printf("==================================================\n");
            printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail + 1);
            printf("  ⚠️ Part C 的测试没有跑完\n");
            printf("==================================================\n\n");
            return 0;
        }
        CHECK(wl->items != NULL,   "items 已分配");
        CHECK(wl->size == 0,       "size = 0");
        CHECK(wl->capacity == 4,   "capacity = 4");
        wl_free(wl);
    }

    /* ---------------- C2. wl_add ---------------- */
    printf("\n---------- C2. wl_add（添加/累加）----------\n");
    {
        WordList *wl = wl_create();

        wl_add(wl, "apple");
        CHECK(wl->size == 1,          "加 1 个词 -> size = 1");
        CHECK(wl_get(wl, "apple") == 1, "apple 的 count = 1");

        wl_add(wl, "apple");
        wl_add(wl, "apple");
        CHECK(wl->size == 1,          "重复加 apple -> size 还是 1");
        CHECK(wl_get(wl, "apple") == 3, "apple 的 count = 3");

        wl_add(wl, "banana");
        CHECK(wl->size == 2,          "加 banana -> size = 2");
        CHECK(wl_get(wl, "banana") == 1, "banana 的 count = 1");
        CHECK(wl_get(wl, "cherry") == -1, "没加过的词 -> -1");

        /* 扩容 */
        wl_add(wl, "c"); wl_add(wl, "d"); wl_add(wl, "e");
        CHECK(wl->size == 5,          "加到 5 个词");
        CHECK(wl->capacity == 8,      "容量从 4 扩到 8");
        CHECK(wl_get(wl, "e") == 1,   "扩容后新词还在");

        wl_free(wl);
    }

    /* ---------------- C3. wl_count_text ---------------- */
    printf("\n---------- C3. wl_count_text（切词）----------\n");
    {
        WordList *wl = wl_create();

        wl_count_text(wl, "the cat the dog the");
        CHECK(wl->size == 3,             "3 个不同的词");
        CHECK(wl_get(wl, "the") == 3,    "the -> 3");
        CHECK(wl_get(wl, "cat") == 1,    "cat -> 1");
        CHECK(wl_get(wl, "dog") == 1,    "dog -> 1");

        wl_free(wl);

        /* 各种空白和标点边界 */
        wl = wl_create();
        wl_count_text(wl, "  a\tb\nc\r\nd  ");
        CHECK(wl->size == 4,          "用空格/tab/换行分隔 -> 4 个词");
        CHECK(wl_get(wl, "a") == 1 && wl_get(wl, "b") == 1 &&
              wl_get(wl, "c") == 1 && wl_get(wl, "d") == 1, "四个词各出现 1 次");

        wl_free(wl);

        /* 空文本 */
        wl = wl_create();
        wl_count_text(wl, "");
        CHECK(wl->size == 0, "空文本 -> 0 个词");
        wl_count_text(wl, "   \n\t  ");
        CHECK(wl->size == 0, "全是空白 -> 0 个词");
        wl_free(wl);
    }

    /* ---------------- C4. wl_sort ---------------- */
    printf("\n---------- C4. wl_sort（按次数排序）----------\n");
    {
        WordList *wl = wl_create();
        wl_add(wl, "b"); wl_add(wl, "a"); wl_add(wl, "c");
        wl_add(wl, "a"); wl_add(wl, "c"); wl_add(wl, "c");
        /* counts: b=1, a=2, c=3 */

        wl_sort(wl);

        CHECK(wl->size == 3, "排序不改变单词个数");
        CHECK(wl->items[0].count == 3, "第 1 名 count = 3");
        CHECK(wl->items[1].count == 2, "第 2 名 count = 2");
        CHECK(wl->items[2].count == 1, "第 3 名 count = 1");
        CHECK(strcmp(wl->items[0].word, "c") == 0, "第 1 名是 c");
        CHECK(strcmp(wl->items[1].word, "a") == 0, "第 2 名是 a");
        CHECK(strcmp(wl->items[2].word, "b") == 0, "第 3 名是 b");

        wl_free(wl);

        /* 次数相同时按字典序 */
        wl = wl_create();
        wl_add(wl, "zebra"); wl_add(wl, "apple");
        wl_sort(wl);
        CHECK(strcmp(wl->items[0].word, "apple") == 0, "次数相同时 apple 在前（字典序）");
        CHECK(strcmp(wl->items[1].word, "zebra") == 0, "zebra 在后");
        wl_free(wl);
    }

    /* ---------------- C5. wl_print_top（人眼检查）---------------- */
    printf("\n---------- C5. wl_print_top（请人眼检查）----------\n");
    {
        WordList *wl = wl_create();
        wl_count_text(wl, "the cat and the dog and the bird");
        wl_sort(wl);

        printf("  实际输出：\n");
        wl_print_top(wl, 3);
        printf("  期望输出：\n");
        printf("     1. the             3\n");
        printf("     2. and             2\n");
        printf("     3. bird            1   （或 cat / dog，字典序 bird 最小）\n");

        wl_free(wl);
    }

    /* ---------------- C6. 端到端：文件 -> 统计 -> Top N ---------------- */
    printf("\n---------- C6. 端到端：读文件 -> 统计 -> Top 3 ----------\n");
    {
        const char *path = "_t_e2e.txt";
        make_file(path, "to be or not to be that is the question\n"
                        "to be or not to be\n");

        int len = 0;
        char *content = file_read_all(path, &len);
        CHECK(content != NULL, "读到文件内容");

        FileStats st;
        CHECK(file_stats(path, &st) == 1, "统计成功");
        printf("  文件统计: %d 行, %d 词, %d 字符\n", st.lines, st.words, st.chars);

        WordList *wl = wl_create();
        if (content != NULL) wl_count_text(wl, content);
        wl_sort(wl);
        CHECK(wl_well_formed(wl), "单词表结构正常");
        CHECK(wl_get(wl, "to") == 4,    "to 出现 4 次");
        CHECK(wl_get(wl, "be") == 4,    "be 出现 4 次");
        CHECK(wl_get(wl, "or") == 2,    "or 出现 2 次");
        CHECK(wl_get(wl, "the") == 1,   "the 出现 1 次");

        printf("  Top 3:\n");
        wl_print_top(wl, 3);

        wl_free(wl);
        if (content) free(content);
        remove(path);
    }

    /* ================= 总结 ================= */
    printf("\n==================================================\n");
    printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail);
    if (g_fail == 0)
        printf("  🎉 全部通过！项目 5 完成！\n");
    else
        printf("  还有 %d 个没通过，继续加油\n", g_fail);
    printf("==================================================\n\n");

    return 0;
}
