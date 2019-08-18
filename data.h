#ifndef DATA_H_INCLUDED
#define DATA_H_INCLUDED

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stack>
#include <set>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

#define NRW        10     // number of reserved words
#define TXMAX      500    // length of identifier table
#define MAXIDLEN   10     // length of identifiers


#define FEOF                   -1
#define ERROR                  -2
#define BEGIN                   1
#define as                      2
#define sub                     3
#define END                     4

#define IF                      5
#define then                    6
#define ELSE                    7
#define call                    8
#define WHILE                   9
#define DO                      10
#define integer                 11
#define FLOAT                   12
#define input                   13
#define output                  14
#define var                     15

#define id                      16
#define digits                  17
#define plus                    18
#define minus                   19
#define multiply                20
#define div                     21
#define LP                      22
#define RP                      23
#define equ                     24
#define comma                   25
#define semi                    26
#define ge                      27
#define le                      28
#define unexpectedFileEnd       29
#define unexpectedStatement     30
#define EE                      31
#define LOE                     32
#define BOE                     33
#define decimal                 34
#define AND                     35
#define OR                      36
#define BL                      37


struct Cond                      //符号表数据结构
{
	string str;
	int type;
	int firstLino;
    bool operator < (const Cond &a) const{
        if(str<a.str){
            return 1;
        }
        else{
            return 0;
        }
    }
    bool operator == (const Cond &a) const{
        return str==a.str;
    }
};
struct Pro
{
    string str;
    int amount;///参数数目
    int pname[100];///参数类型
};
extern void program();
extern int getToken();
extern char * getID(int index);
extern bool enter(char name[]);
extern int lookup(char name[]);

#endif // DATA_H_INCLUDED
