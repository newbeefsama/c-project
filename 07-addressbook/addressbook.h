#ifndef ADDRESSBOOK_H
#include<stdio.h>
#include<stdlib.h>
#define ADDRESSBOOK_H

#define name_max 64

#define capacity 32

#define phone_max 32

#define max 32

typedef struct{
    char phone_number[max];
    char name[name_max];
}connect_person;

typedef struct{
    connect_person *  list;
    int size;
    int cap;
}address_list;

address_list* create_list();
void research_file(void);
address_list* push_list(address_list* al);
int Continue(void);
int maun(void);
void research_file(void);
void delete_file(void);
void read_file(void);
void push_file(address_list* al,int size);
void chomp(char *s);

#endif


