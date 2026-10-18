#ifndef TEXT_H
#define TEXT_H

/* ============================================================
 *  项目 5：文本统计（文件操作）
 *
 *  你会第一次接触：fopen / fgetc / fgets / fputs / fclose
 *
 *  ★ 核心认知：
 *      文件操作和内存操作是【两套世界】。
 *      内存里的数据，程序一结束就没了；
 *      文件里的数据，下次启动还在。
 * ============================================================ */


/* ============================================================
 *  Part A：文件基础
 * ============================================================ */

/* A1. 读取整个文件到内存
       成功：返回 malloc 出来的字符串（以 '\0' 结尾）
             *out_len 写入字符个数（不含 '\0'）
       失败：返回 NULL（文件不存在 / 读失败 / 内存不足）
       ⚠️ 调用者负责 free 返回的指针
       ⚠️ *out_len 可能是 NULL（调用者不关心长度时） */
char *file_read_all(const char *path, int *out_len);

/* A2. 把字符串写入文件（覆盖写：原来的内容会被清空）
       成功返回 1，失败返回 0 */
int file_write_all(const char *path, const char *content);

/* A3. 复制文件 src -> dst
       成功返回 1，失败返回 0 */
int file_copy(const char *src, const char *dst);


/* ============================================================
 *  Part B：统计
 * ============================================================ */

typedef struct {
    int lines;      /* 行数 */
    int words;      /* 单词数 */
    int chars;      /* 字符数（换行符也算 1 个字符） */
} FileStats;

/* B1. 统计文件
       ★ 精确定义（测试按这个来）：
          chars = 文件里字符的总个数（'\n' 也算 1 个）
          lines = '\n' 的个数
                  + （文件非空 且 最后一个字符不是 '\n' 时加 1）
          words = 连续的非空白字符段数
                  空白 = ' ' '\t' '\n' '\r' '\v' '\f'
       成功返回 1 并把结果写进 *out；失败返回 0 */
int file_stats(const char *path, FileStats *out);


/* ============================================================
 *  Part C：单词计数（把前面学的动态数组用上）
 * ============================================================ */

#define WORD_MAX 32

typedef struct {
    char word[WORD_MAX];    /* 单词本身（超长截断） */
    int  count;             /* 出现次数 */
} WordCount;

typedef struct {
    WordCount *items;
    int        size;
    int        capacity;
} WordList;

/* C1. 创建空单词表（初始容量 4，和动态数组一样） */
WordList *wl_create(void);

/* C2. 添加一个单词
       - 已经存在（strcmp 相等）→ count++
       - 不存在 → 追加到末尾，count = 1
       - 满了 → 容量翻倍
       ⚠️ 不许用 <string.h> 的 strcpy / strcmp
          （你可以自己写，或者用之前项目 1 的思路） */
void wl_add(WordList *wl, const char *word);

/* C3. 把一段文本切分成单词，逐个 wl_add
       单词 = 连续的非空白字符
       空白 = ' ' '\t' '\n' '\r' '\v' '\f' */
void wl_count_text(WordList *wl, const char *text);

/* C4. 按 count 从大到小排序（冒泡排序就行）
       count 相同时按单词字典序排（保证结果稳定） */
void wl_sort(WordList *wl);

/* C5. 打印前 n 名
       n 超过实际单词数就打印全部
       格式（注意对齐）：
           1. the         12
           2. is           8
           3. a            7 */
void wl_print_top(WordList *wl, int n);

/* C6. 释放 */
void wl_free(WordList *wl);

#endif  /* TEXT_H */
