#include "my_string.h"

/* ============================================================
 *  ★★★ 这是你要改的文件 ★★★
 *
 *  把下面 7 个函数的函数体写出来。
 *  规则：不许 #include <string.h>
 *
 *  写完一个就跑一次 run.bat，看测试结果。
 * ============================================================ */


/* ---------- 1. 算长度（不含结尾的 '\0'） ---------- */
size_t my_strlen(const char*s)
{
    int i=0;
    for(;s[i]!='\0';i++)
    ;
    return i;
}


/* ---------- 2. 复制（含结尾的 '\0'），返回 dest ---------- */
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
    /* TODO:
       1. 用一个下标 i 从 0 开始
       2. 每轮：dest[i] = src[i]
       3. 遇到 src[i] == '\0' 时，把 dest[i] 也置为 '\0'，结束
       4. 返回 dest
    */
    return dest;
}


/* ---------- 3. 拼接：把 src 接到 dest 末尾，返回 dest ---------- */
char *my_strcat(char *dest, const char *src)
{
    int i=0;
    for(;dest[i]!='\0';i++)
    ;
for(int j=0;src[j]!='\0';j++,i++)
{
dest[i]=src[j];
}
dest[i]='\0';
    /* TODO:
       1. 先找到 dest 的末尾（'\0' 的位置），记它的下标为 i
       2. 从 i 开始，把 src 的字符（含 '\0'）复制过去
       3. 返回 dest
    */
    return dest;
}


/* ---------- 4. 比较：s1<s2 返回负数，相等 0，s1>s2 返回正数 ---------- */
int my_strcmp(const char *s1, const char *s2)
{
    int s;
    int n,m;
    n=my_strlen(s1);
    m=my_strlen(s2);
    for(int i=0,j=0;i<=n||j<=m;j++,i++)
    {
        s=s1[i]-s2[j];
        if(s!=0)
        {
            return s;
        }
    }
    /* TODO:
       1. 同时走两个指针
       2. 如果当前字符不同，返回它们相减的结果（正负都行，只要符号对）
       3. 如果两个都到了 '\0'，返回 0
    */
    return 0;
}


/* ---------- 5. 原地反转字符串 ---------- */
void my_strrev(char *s)
{
    int len;
    len=my_strlen(s);
        for(int i=0;i<len/2;i++)
        {
            char c;
            c=s[i];
            s[i]=s[len-i-1];
            s[len-i-1]=c;
        }
}


/* ---------- 6. 查找字符（找不到返回 NULL） ---------- */
char *my_strchr(const char *s, int c)
{
    char* p=(char*)s;
    /* TODO:
       1. 从头扫到尾（包括结尾的 '\0'）
       2. 如果 *p == (char)c，返回 p
       3. 如果始终没找到，返回 NULL
       ⚠️ 注意：要能查到结尾的 '\0'（返回指向它的指针）
       ⚠️ 返回类型是 char*，但 s 是 const char*，需要转换
    */
    int len=my_strlen(s);
    for(int i=0;i<=len;i++,p++)
    {
        if(s[i]==c)
        {
return p;
        }
    }
    return NULL;
}


/* ---------- 7. 字符串转整数（最难的一个） ---------- */
int my_atoi(const char *s)
{
    int p=0;
    int len=my_strlen(s);
    int sign=1;
    int d=0;
    for(int i=0;i<len;i++)
    {
        if(s[i]=='-')
        {
            sign=-1;
            d++;
        }
        else if(s[i]<='9'&&s[i]>='0')
        {
p=p*10+(s[i]-'0');
        }
        else if((s[i]<91&&s[i]>64)||(s[i]>96&&s[i]<123))
        {
            d++;
break;
        }
        else{
            d++;
        }
    }
    if(d!=len)
    {
        p=p*sign;
return p;
    }
    /* TODO:
       步骤：
       1. 跳过前导空白（' '、'\t'、'\n'）
       2. 处理正负号（记录 sign = 1 或 -1）
       3. 循环：只要当前字符是数字（'0'~'9'）
            result = result * 10 + (c - '0');
       4. 返回 sign * result
       ⚠️ "abc" 要返回 0，"12ab" 要返回 12
    */
    return 0;
}
