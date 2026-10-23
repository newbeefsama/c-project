#include <stdio.h>
#include <stdlib.h>
#include "text.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  10 个函数，分三部分：
 *      Part A  文件基础（读、写、复制）
 *      Part B  统计（行/词/字符）
 *      Part C  单词计数（用动态数组）
 *
 *  ⚠️ 文件操作的三条铁律：
 *      1. fopen 可能返回 NULL（文件不存在 / 没权限）→ 必须检查
 *      2. 打开的文件必须 fclose（否则叫"文件句柄泄漏"）
 *      3. fgetc 返回 int（不是 char）—— 因为 EOF 是 -1，char 装不下
 * ============================================================ */


/* ============================================================
 *  Part A：文件基础
 * ============================================================ */
char *file_read_all(const char *path, int *out_len)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) return NULL;

    int cap = 64, size = 0;
    char *buf = (char *)malloc(cap);
    if (buf == NULL) { fclose(fp); return NULL; }

    int c;
    while ((c = fgetc(fp)) != EOF)     
    {
        if (size + 1 >= cap)            
        {
            int newcap = cap * 2;
            char *tmp = (char *)realloc(buf, newcap);
            if (tmp == NULL) { free(buf); fclose(fp); return NULL; }
            buf = tmp;                 
            cap = newcap;
        }
        buf[size++] = (char)c;
    }
    buf[size] = '\0';                   
    fclose(fp);

    if (out_len != NULL) *out_len = size;   
    return buf;
}
    /* TODO:
       1. FILE *fp = fopen(path, "r");
          if (fp == NULL) return NULL;
       2. 准备一个动态缓冲区（初始 64 字节）：
             int cap = 64, size = 0;
             char *buf = malloc(cap);
             if (buf == NULL) { fclose(fp); return NULL; }
       3. 循环读字符：
             int c;
             while ((c = fgetc(fp)) != EOF) {
                 if (size + 1 >= cap) {        ← 留一格放 '\0'
                     cap *= 2;
                     char *tmp = realloc(buf, cap);
                     if (tmp == NULL) { free(buf); fclose(fp); return NULL; }
                     buf = tmp;
                 }
                 buf[size++] = (char)c;
             }
       4. buf[size] = '\0';
          fclose(fp);
          if (out_len != NULL) *out_len = size;
          return buf;
       ⚠️ 所有失败路径都要 fclose + free 已申请的内存
       ⚠️ fgetc 的返回值要用 int 接，不能写 char c
    */


/* A2. 写文件 */
int file_write_all(const char *path, const char *content)
{
    FILE* fp=fopen(path,"w");
    if(fp==NULL)
    {
        return 0;
    }
    fputs(content,fp);
    int ok=(fclose(fp)==0);
    return ok;
    /* TODO:
       1. FILE *fp = fopen(path, "w");     ← "w" 是覆盖写
          if (fp == NULL) return 0;
       2. fputs(content, fp);
       3. int ok = (fclose(fp) == 0);      ← fclose 也可能失败！
      return ok;
    */
    return 0;
}

/* A3. 复制文件 */
int file_copy(const char *src, const char *dst)
{
    char* a=file_read_all(src,0);
    if(a==NULL)
    {
        return 0;
    }
    file_write_all(dst,a);
    free(a);

    /* TODO:
       方法 1（简单粗暴）：用你写的 file_read_all + file_write_all
       方法 2（更专业）：一块一块地读（比如每次 1024 字节）
       建议先用方法 1，做完再想想方法 2 的好处（大文件不会撑爆内存）
       ⚠️ 记得释放 file_read_all 返回的指针
    */
    return 1;
}


/* ============================================================
 *  Part B：统计
 * ============================================================ */

/* B1. 统计文件 */
int file_stats(const char *path, FileStats *out)
{
    FILE* fp=fopen(path,"r");
    if(out==NULL)
    {
        return 0;
    }
    if(fp==NULL)
    {
        return 0;
    }
    int lines=0,words=0,chars=0,last=-1,in_word=0;
    int c;
    while((c=fgetc(fp))!=EOF)
    {
        chars++;
        if(c=='\n')
        {
            lines++;
        }
        if(c==' '||c=='\t'||c=='\r'||c=='\f'||c=='\v'||c=='\n')
        {
            in_word=0;
        }
        else{
            if(in_word==0)
            {
                words++;
                in_word=1;
            }
        }
        last=c;
    }
    if(chars>0&&last!='\n')
    {lines++;}
    fclose(fp);
    out->lines=lines;
    out->words=words;
    out->chars=chars;

    /* TODO:
       FILE *fp = fopen(path, "r");
       if (fp == NULL || out == NULL) { if (fp) fclose(fp); return 0; }

       int lines = 0, words = 0, chars = 0;
       int in_word = 0;          ← 0 = 当前不在单词里，1 = 在单词里
       int last = -1;            ← 上一个字符（用来判断结尾）
       int c;

       while ((c = fgetc(fp)) != EOF) {
           chars++;
           if (c == '\n') lines++;

           if (c==' '||c=='\t'||c=='\n'||c=='\r'||c=='\v'||c=='\f') {
               in_word = 0;                          ← 遇到空白，离开单词
           } else {
               if (!in_word) { words++; in_word = 1; }  ← 从空白进入单词
           }
           last = c;
       }

       ← 最后一行没有换行符结尾时，要补 1 行
       if (chars > 0 && last != '\n') lines++;

       fclose(fp);
       out->lines = lines;  out->words = words;  out->chars = chars;
       return 1;
    */
    return 1;
}


/* ============================================================
 *  Part C：单词计数
 * ============================================================ */

#define WL_INIT_CAPACITY 4

/* C1. 创建空单词表 */
WordList *wl_create(void)
{
    WordList* p=malloc(sizeof(WordList));
    if(p==NULL)
    {
        return NULL;
    }
      p->size=0;
     p->capacity=WL_INIT_CAPACITY;
     p->items=malloc(sizeof(WordCount)*WL_INIT_CAPACITY);
    if(p->items==NULL)
    {
        return NULL;
    }
    /* TODO: 和 da_create 一样（两次 malloc、都判空、初始容量 4）
       ⚠️ items 是 WordCount 数组，malloc 的大小是 4 * sizeof(WordCount) */
    return p;
}

/* C2. 添加单词 */
int my_strcmp(const char* a,const char* b)
{int s;
    int j=0,i=0;
    for(;a[i]!='\0'&&b[j]!='\0';j++,i++)
    {
        s=a[i]-b[j];
        if(a[i]-b[j]!=0)
        {
            return s;
        }
    }
    s=a[i]-b[j];
    return s;
}
char *my_strcpy(char *dest, const char *src)
{
    int i=0;
    for(;src[i]!='\0';i++)
    {
        dest[i]=src[i];
    }
    if(src[i]=='\0')
    {
        dest[i]='\0';
    }
    return dest;
}
void wl_add(WordList *wl, const char *word)
{
    for(int i=0;i<wl->size;i++)
    {
        if(my_strcmp(word,wl->items[i].word)==0)
        {
            wl->items[i].count++;
            return;
        }
    }
    if(wl->size==wl->capacity)
    {
        int new=0;
        new=wl->capacity*2;
        WordCount *temp=realloc(wl->items,sizeof(WordCount)*new);
        if(temp==NULL)
        {
            return;
        }
        wl->items=temp;
        wl->capacity*=2;}
        my_strcpy(wl->items[wl->size].word,word);
        wl->items[wl->size].count=1;
        wl->size++;
    /* TODO:
       1. 先找：遍历 items，用 strcmp 比较 word。
          如果找到 → items[i].count++;  return;
       2. 没找到 → 需要新增
          a. 如果 size == capacity → 扩容（翻倍，realloc 先存 tmp）
          b. 把 word 复制进 items[size].word
             ⚠️ 不能用 strcpy（题目禁止），自己写个循环复制
             ⚠️ 注意别超过 WORD_MAX-1，最后一个位置留给 '\0'
          c. items[size].count = 1;
          d. size++;
    */
}

/* C3. 把文本切分成单词 */
void wl_count_text(WordList *wl, const char *text)
{
    char buf[WORD_MAX];
    int len=0;
    if(wl==NULL||text==NULL)
    {
        return;
    }
    for(int i=0;;i++)
    {
        char ch=text[i];
if(ch=='\0'||ch==' '||ch=='\t'||ch=='\r'||ch=='\f'||ch=='\v'||ch=='\n')
{
    if(len>0)
    {
buf[len]='\0';
wl_add(wl,buf);
len=0;
}
if(ch=='\0')
{
    break;
}
}
else
{
    if(len<WORD_MAX-1)
    {
        buf[len++]=ch;
    }
}
    /* TODO:
       用一个 char buf[WORD_MAX] 当"临时单词"，
       遍历 text：
         非空白 → 塞进 buf（注意别超长）
         空白   → 如果 buf 非空，buf 结尾加 '\0'，wl_add(wl, buf)，然后清空 buf
       循环结束后，buf 里可能还有最后一个单词 → 别忘了 wl_add
    */
}
}

/* C4. 按次数排序 */
int my_strcmp2(const char* a,const char* b)
{
    while(*a!='\0'&&*a-*b==0)
    {
        a++;
        b++;
        if(*a-*b!=0)
        {
            return *a-*b;
        }
        
    }
    return *a-*b;
}
void wl_sort(WordList *wl)
{
    for(int j=0;j<wl->size;j++)
    {
        for(int i=0;i<wl->size-j-1;i++)
        {
            if(wl->items[i].count<wl->items[i+1].count)
            {
            WordCount s=wl->items[i];
            wl->items[i]=wl->items[i+1];
            wl->items[i+1]=s;
            }
            if(wl->items[i].count==wl->items[i+1].count)
            {
                int s=my_strcmp2(wl->items[i].word,wl->items[i+1].word);
                if(s>0)
                {
            WordCount s=wl->items[i];
            wl->items[i]=wl->items[i+1];
            wl->items[i+1]=s;
                }
            }
        }
    }
    /* TODO: 冒泡排序
       for (int i = 0; i < wl->size - 1; i++)
           for (int j = 0; j < wl->size - 1 - i; j++)int s=my_strcmp2(wl->items[i].word,wl->items[i+1].word);
        if(s<0)
        {
            WordCount s=wl->items[i];
            wl->items[i]=wl->items[i+1];
            wl->items[i+1]=s;
        }
               if (需要交换) { 交换 items[j] 和 items[j+1] }
       ⚠️ 交换的是整个 WordCount 结构体（含 word 数组 + count）
          不能只交换指针——items 是结构体数组，不是指针数组
       排序规则：count 大的在前；count 相同按单词字典序（strcmp < 0 在前）
       ⚠️ 比较单词用 strcmp，虽然不能用 string.h 的复制，但可以自己写比较
    */
}

/* C5. 打印前 n 名 */
void wl_print_top(WordList *wl, int n)
{
    for(int i=0;i<n;i++)
    {
        for(int j=0;wl->items[i].word[j]!='\0';j++)
        {
            printf("%c",wl->items[i].word[j]);
        }
        printf("\n");
    }
    /* TODO:
       要打印的个数 = min(n, wl->size)
       格式：printf("%4d. %-12s %4d\n", i+1, wl->items[i].word, wl->items[i].count);
       ⚠️ %-12s 表示左对齐宽度 12 —— 用来对齐
    */
}

/* C6. 释放 */
void wl_free(WordList *wl)
{
    if(wl==NULL)
    {
        return ;
    }
    free(wl->items);
    free(wl);
    /* TODO: if (wl == NULL) return;  free(wl->items);  free(wl); */
}
