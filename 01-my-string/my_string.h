#ifndef MY_STRING_H
#define MY_STRING_H

/* ============================================================
 *  函数声明 —— 这个文件不要动！
 *  你只需要在 my_string.c 里实现这些函数
 * ============================================================ */

#include <stddef.h>     /* size_t 需要它 */

/* 算字符串长度（不含结尾的 '\0'） */
size_t my_strlen(const char *s);

/* 把 src 复制到 dest（含 '\0'），返回 dest */
char *my_strcpy(char *dest, const char *src);

/* 把 src 拼接到 dest 末尾，返回 dest */
char *my_strcat(char *dest, const char *src);

/* 比较两个字符串：s1<s2 返回负数，相等返回 0，s1>s2 返回正数 */
int my_strcmp(const char *s1, const char *s2);

/* 原地反转字符串 */
void my_strrev(char *s);

/* 在 s 中查找字符 c，返回第一次出现的位置；找不到返回 NULL */
char *my_strchr(const char *s, int c);

/* 字符串转整数，跳过前导空白和正负号 */
int my_atoi(const char *s);

#endif  /* MY_STRING_H */
