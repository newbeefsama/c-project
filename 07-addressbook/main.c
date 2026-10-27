#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "addressbook.h"

int main()
{
    address_list* al=create_list();
    al->size=0;
    al->cap=capacity;
int jugde=0;
    do
    {
    int ret=maun();
    switch(ret)
    {
        case 0:
        {
            exit(0);
        }
        case 1:
        {
            int a=al->size;
            al=push_list(al);
            push_file(al,a);
            jugde=Continue();
            break;
        }
        case 2:
        {
            research_file();
            jugde=Continue();
            break;
        }
         case 3:
        {
            delete_file();
            jugde=Continue();
            break;
        }
         case 4:
        {
            read_file();
            jugde=Continue();
            break;
        }
    }
}while(jugde!=0);
    return 0;
}

address_list* create_list()
{
    address_list* al=malloc(sizeof(address_list));
    if(al==NULL)
    {
        printf("error");
        return NULL;
    }
    al->list=malloc(sizeof(connect_person)*capacity);
    if(al->list==NULL)
    {
        printf("error");
        return NULL;
    }
    return al;
}

address_list* push_list(address_list* al)
{
    int n;
    printf("how many people do you want to input:");
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {if(al->size==al->cap)
    {
        int new;
        new=al->cap*2;
        connect_person* tmp=realloc(al->list,sizeof(connect_person)*new);
        if(tmp==NULL)
        {
            printf("error");
            return NULL;
        }
        al->cap=new;
        al->list=tmp;
    }
    printf("请输入联系人的名字：");
    scanf("%s",al->list[al->size].name);
    printf("请输入联系人的电话：");
    scanf("%s",al->list[al->size].phone_number);
    al->size++;}
    return al;
}

void push_file(address_list* al,int size)
{
    FILE* fp1=fopen("addressbookname.txt","a");
    FILE* fp2=fopen("addressbooknumber.txt","a");
    for(int i=size;i<al->size;i++)
    {
        fprintf(fp1,"%s\n",al->list[i].name);
        fprintf(fp2,"%s\n",al->list[i].phone_number);
    }
    fclose(fp1);
    fclose(fp2);
    return ;
}

int Continue()
{
    int judge;
    char a;
    printf("do you want to continue?(y/n)\n");
    do{
    scanf(" %c",&a);
    if (a=='y')
    {
        judge=1;
    }
    else if(a=='n')
    {
        judge=0;
    }
else
{
    judge=-1;
}
}while(judge<0);
    return judge;
}

void research_file()
{
    char s[name_max];
    char n1[name_max];
    char n2[phone_max];
    int i=0;
    int j=0;
    FILE* fp1=fopen("addressbookname.txt","r");
    if(fp1==NULL)
    {
        printf("Can't open then file\n");
        return ;
    }
    FILE* fp2=fopen("addressbooknumber.txt","r");
    if(fp2==NULL)
    {
        printf("Can't open the file\n");
        return ;
    }
    printf("who do you want to research:");
    fgets(s,sizeof(s),stdin);
    chomp(s);
    int flag=0;
    while(fgets(n1,sizeof(n1),fp1)!=NULL)
    {
        i++;
        chomp(n1);
        if(strcmp(n1,s)==0)
        {
            flag=1;
            break;
        }
    }
    if(flag!=1)
    {
    printf("the person is not in your addressbook\n");
    fclose(fp1);
    fclose(fp2);
    return ;
    }
    while(fgets(n2,sizeof(n2),fp2)!=NULL)
    {
        j++;
        chomp(n2);
        if(j==i)
        {
            printf("her/his number is %s\n",n2);
            fclose(fp1);
            fclose(fp2);
            return ; 
        }
    }
    return ;
}

void delete_file()
{
    FILE* fp01=fopen("tmp1.txt","w");
    if(fp01==NULL)
    {
        printf("create fail\n");
        return ;
    }
    FILE* fp1=fopen("addressbookname.txt","r");
    if(fp1==NULL)
    {
        printf("no information");
        return ;
    }
    FILE * fp2=fopen("addressbooknumber.txt","r");
    if(fp2==NULL)
    {
        printf("no information");
        return ;
    }
    FILE * fp02=fopen("tmp2.txt","w");
    if(fp02==NULL)
    {
        printf("create fail\n");
        return ;
    }
    if(fp02==NULL)
    {
        printf("create error\n");
        return ;
    }
    char number1[max];
    char name[name_max];
    char name0[name_max];
    printf("which one do you want to delete:");
    fgets(name,sizeof(name),stdin);
    chomp(name);
    char n1[name_max];
    int i=0;
    int je=0;
    int jee=0;
    if(fp1==NULL)
    {
        printf("Can't open then file\n");
        return ;
    }
    if(fp2==NULL)
    {
        printf("Can't open the file\n");
        return ;
    }
    int flag=0;
    while(fgets(n1,sizeof(n1),fp1)!=NULL)
    {
        chomp(n1);
        i++;
        if(strcmp(n1,name)==0)
        {
            flag=1;
            break;
        }
    }
    if(flag!=1)
    {
    printf("the person is not in your addressbook\n");
    fclose(fp1);
    fclose(fp2);
    return ;}
    rewind(fp1);
    while(fgets(number1,sizeof(number1),fp2)!=NULL)
    {
        je++;
        if(je!=i)
        {
            fprintf(fp02,"%s",number1);
        }
    }
    while(fgets(name0,sizeof(name0),fp1)!=NULL)
    {
        jee++;
        if(jee!=i)
        {
            fprintf(fp01,"%s",name0);
        }
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp01);
    fclose(fp02);
    if(remove("addressbookname.txt")!=0)
    {
        printf("delete error\n");
    }
    if(remove("addressbooknumber.txt")!=0)
    {
        printf("delete error\n");
    }
    if(rename("tmp1.txt","addressbookname.txt")!=0)
    {
        printf("delete error\n");
    }
    if(rename("tmp2.txt","addressbooknumber.txt")!=0)
    {
        printf("delete error\n");
    }
    return;
}

void read_file()
{
    FILE* fp1=fopen("addressbookname.txt","r");
    if(fp1==NULL)
    {
        printf("no information");
        return ;
    }
    FILE* fp2=fopen("addressbooknumber.txt","r");
    if(fp2==NULL)
    {
        printf("no information");
        return ;
    }
    char name[name_max];
    char number[max];
    while(fgets(name,sizeof(name),fp1)!=NULL&&fgets(number,sizeof(number),fp2)!=NULL)
    {
        chomp(name);
        chomp(number);
        printf("%s\t%s\n",name,number);
    }
    fclose(fp1);
    fclose(fp2);
    return ;
}

int maun(void)
{
    printf("退出请输入0\n");
    printf("添加请输入1\n");
    printf("查找请输入2\n");
    printf("删除请输入3\n");
    printf("显示全部请输入4\n");
    int a;
    scanf("%d",&a);
    getchar();
    return a;
}
void chomp(char *s)
{
    s[strcspn(s, "\n")] = '\0';
}