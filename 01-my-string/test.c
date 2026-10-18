#include <stdio.h>
#include "my_string.h"

/* ============================================================
 *  测试程序 —— 这个文件不要动！
 *  你只要让所有 [FAIL] 变成 [PASS] 就行
 * ============================================================ */

static int g_pass = 0;
static int g_fail = 0;

static void check(int cond, const char *msg, int line)
{
    if (cond) { g_pass++; printf("  [PASS] %s\n", msg); }
    else      { g_fail++; printf("  [FAIL] %s        <-- test.c 第 %d 行\n", msg, line); }
}

#define CHECK(cond, msg) check((cond), (msg), __LINE__)

/* 测试用的字符串比较（自己写，不依赖 string.h） */
static int same(const char *a, const char *b)
{
    while (*a != '\0' && *a == *b) { a++; b++; }
    return *a == *b;
}

int main(void)
{
    printf("\n");
    printf("##################################################\n");
    printf("#   项目 1：字符串函数库 —— 自动测试              #\n");
    printf("##################################################\n");

    /* ---------------- 1. my_strlen ---------------- */
    printf("\n---------- 1. my_strlen（算长度）----------\n");
    CHECK(my_strlen("") == 0,              "空串 -> 0");
    CHECK(my_strlen("a") == 1,             "\"a\" -> 1");
    CHECK(my_strlen("ab") == 2,            "\"ab\" -> 2");
    CHECK(my_strlen("hello") == 5,         "\"hello\" -> 5");
    CHECK(my_strlen("hello world") == 11,  "\"hello world\" -> 11");
    CHECK(my_strlen(" ") == 1,             "一个空格 -> 1");

    /* ---------------- 2. my_strcpy ---------------- */
    printf("\n---------- 2. my_strcpy（复制）----------\n");
    {
        char buf[100];
        char *r;

        sprintf(buf, "xxxxxxxxxx");
        r = my_strcpy(buf, "hello");
        CHECK(same(buf, "hello"), "复制 \"hello\"");
        CHECK(r == buf,           "返回值是 dest 本身");

        sprintf(buf, "xxxxxxxxxx");
        my_strcpy(buf, "");
        CHECK(same(buf, ""),      "复制空串");

        sprintf(buf, "xxxxxxxxxx");
        my_strcpy(buf, "a");
        CHECK(same(buf, "a"),     "复制单字符");

        /* ★ 哨兵检查：目标串结尾之后有没有被写到？ */
        {
            char s2[50];
            for (int k = 0; k < 50; k++) s2[k] = 'X';
            my_strcpy(s2, "abc");
            CHECK(s2[4] == 'X',   "没有越界写入（哨兵检查）");
        }
    }

    /* ---------------- 3. my_strcat ---------------- */
    printf("\n---------- 3. my_strcat（拼接）----------\n");
    {
        char buf[100];
        char *r;

        sprintf(buf, "hello");
        r = my_strcat(buf, " world");
        CHECK(same(buf, "hello world"), "拼接 \" world\"");
        CHECK(r == buf,                 "返回值是 dest 本身");

        buf[0] = '\0';
        my_strcat(buf, "abc");
        CHECK(same(buf, "abc"),         "空串 + \"abc\"");

        sprintf(buf, "abc");
        my_strcat(buf, "");
        CHECK(same(buf, "abc"),         "\"abc\" + 空串");

        /* ★ 哨兵检查：目标串结尾之后有没有被写到？ */
        {
            char s2[50];
            for (int k = 0; k < 50; k++) s2[k] = 'X';
            s2[0] = '\0';
            my_strcat(s2, "abc");
            CHECK(s2[4] == 'X',             "没有越界写入（哨兵检查）");
        }
    }

    /* ---------------- 4. my_strcmp ---------------- */
    printf("\n---------- 4. my_strcmp（比较）----------\n");
    CHECK(my_strcmp("abc", "abc") == 0,  "相等 -> 0");
    CHECK(my_strcmp("", "") == 0,        "两个空串 -> 0");
    CHECK(my_strcmp("abc", "abd") < 0,   "\"abc\" < \"abd\"");
    CHECK(my_strcmp("abd", "abc") > 0,   "\"abd\" > \"abc\"");
    CHECK(my_strcmp("ab", "abc") < 0,    "\"ab\" < \"abc\"（前缀）");
    CHECK(my_strcmp("abc", "ab") > 0,    "\"abc\" > \"ab\"");
    CHECK(my_strcmp("", "a") < 0,        "空串 < \"a\"");
    CHECK(my_strcmp("a", "") > 0,        "\"a\" > 空串");
    CHECK(my_strcmp("ABC", "abc") < 0,   "大写 ASCII 更小");
    CHECK(my_strcmp("A", "Z") < 0,       "\"A\" < \"Z\"");

    /* ---------------- 5. my_strrev ---------------- */
    printf("\n---------- 5. my_strrev（反转）----------\n");
    {
        char b1[] = "hello";
        char b2[] = "a";
        char b3[] = "";
        char b4[] = "ab";
        char b5[] = "abcd";

        my_strrev(b1);
        CHECK(same(b1, "olleh"), "\"hello\" -> \"olleh\"");

        my_strrev(b2);
        CHECK(same(b2, "a"),     "单字符不变");

        my_strrev(b3);
        CHECK(same(b3, ""),      "空串不变");

        my_strrev(b4);
        CHECK(same(b4, "ba"),    "\"ab\" -> \"ba\"");

        my_strrev(b5);
        CHECK(same(b5, "dcba"),  "\"abcd\" -> \"dcba\"");
    }

    /* ---------------- 6. my_strchr ---------------- */
    printf("\n---------- 6. my_strchr（查找字符）----------\n");
    {
        const char *s = "hello";

        CHECK(my_strchr(s, 'h') == s,       "'h' 在开头");
        CHECK(my_strchr(s, 'l') == s + 2,   "'l' 返回第一个（下标 2）");
        CHECK(my_strchr(s, 'o') == s + 4,   "'o' 在末尾（下标 4）");
        CHECK(my_strchr(s, 'z') == NULL,    "找不到 -> NULL");
        CHECK(my_strchr("", 'a') == NULL,   "空串 -> NULL");
        CHECK(my_strchr(s, '\0') == s + 5,  "查找 '\\0' 返回指向结尾的指针");
        {
            char *p = my_strchr(s, 'l');
            CHECK(p != NULL && *p == 'l',   "返回的指针可以解引用");
        }
    }

    /* ---------------- 7. my_atoi ---------------- */
    printf("\n---------- 7. my_atoi（字符串转整数）----------\n");
    CHECK(my_atoi("123") == 123,               "\"123\" -> 123");
    CHECK(my_atoi("0") == 0,                   "\"0\" -> 0");
    CHECK(my_atoi("-45") == -45,               "\"-45\" -> -45");
    CHECK(my_atoi("+7") == 7,                  "\"+7\" -> 7");
    CHECK(my_atoi("  789") == 789,             "跳过前导空格");
    CHECK(my_atoi("12ab") == 12,               "遇到非数字就停");
    CHECK(my_atoi("abc") == 0,                 "没有数字 -> 0");
    CHECK(my_atoi("") == 0,                    "空串 -> 0");
    CHECK(my_atoi("   -42xyz") == -42,         "空格+负号+非数字");
    CHECK(my_atoi("2147483647") == 2147483647, "INT_MAX");

    /* ---------------- 总结 ---------------- */
    printf("\n==================================================\n");
    printf("  测试结果：  PASS = %d   FAIL = %d\n", g_pass, g_fail);
    if (g_fail == 0)
        printf("  🎉 全部通过！项目 1 完成！\n");
    else
        printf("  还有 %d 个没通过，继续加油\n", g_fail);
    printf("==================================================\n\n");

    return 0;
}
