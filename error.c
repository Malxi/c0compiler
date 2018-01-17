#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"

char errmsg[100][100] = {
    "illegal character",                                    /* 出现非法字符 */
    "identifier is too long",                               /* 标识符过长 */
    "integer constant is too large for its type",           /* 整型常量过大 */
    "character constant too long for its type",             /* 字符常量过长 */
    "missing terminating ' character",                      /* 字符定义缺少 ' */
    "missing terminating \" character",                     /* 字符串定义缺少 " */
    "string constant too long",                             /* 字符常量过长 */

    "expected identifier",                                  /* 缺少标识符 int  */
    "expected type",                                        /* 缺少类型关键字 const a = 3; */
    "expected integer constant",                            /* 缺少整型常量 const int a = 'a'; */
    "expected char constant",                               /* 缺少字符常量 const char a = -2; */
    "expected constant",                                    /* 缺少常量 */
    "expected 'const', 'int', 'char', 'void'",              /* 缺少程序后继 progra */
    "expected ',', ';'",                                    /* 缺少, ; 符号 int a */
    "expected '='",                                         /* 缺少 = 符号 const int a ;*/
    "expected '('",                                         /* if) or while)... */
    "expected ')'",
    "expected '{'",
    "expected '}'",                                         /* int func{ */
    "expected ':'",                                         /* case 1 {} */
    "expected expression",                                  /* 缺少表达式 a = ; or return ()*/            
    "expected statement",                                   /* 缺少语句 if(a==b); */
    //"expected '==', '<', '<=', '>', '>=', '!='",            /* 缺少逻辑运算符 if(a=b){} */

    "array bounds missing '['",                             /* 数组下标缺少[ a */
    "array bounds missing ']'",                             /* 数组下标缺少] int a[12 */
    "array bounds missing unsigned integer",                /* 数组下标缺少无符号整型 int a[] */
    "array bounds out of index",                            /* 数组下标超出 */

    "identifier redeclaration",                             /* 重复声明 int a; char a */                      
    //"function missing compoundstatement",                 /* int func(int a); or void func; */
    "function should return value",                         /* 函数需要return语句  */
    "function may not return a value",                      /* 函数多出return语句 */
    
    "identifier undeclared (first use in this function)",   /* 标识符未声明 */
    "assignment of read-only variable",                     /* 常量被赋值 */
    "if statement missing 'else'",                          /* if缺少else if(a == 1){} */
    "'else' without a previous 'if'",                       /* else 位置错误 */
    "case missing ':'",                                     /* case缺少 : 符号 case 1 {} */
    "case missing constant",                                /* case缺少常量 case a:{} */
    "case missing statement",                               /* case缺少语句 case 1 : ; */
    //"case duplicate",                                       /* case常量重复 case 1:{}case 1:{}*/
    //"case too many ",                                       /* case太多 */
    "error: case label not within a switch statement",      /* case 错误位置 */
    "default missing ':'",                                  /* default 1 {} */
    "'default' label not within a switch statement",        /* default 错误位置 */
    "expression syntax",                                    /* 表达式语法错误 a + + b */
    "division by zero",                                     /* 表达式除0 a / 0 */
    "too few parameters in call",                           /* 函数调用参数过少 */
    "extra parameter in call",                              /* 函数调用参数过多 */
    "type mismatch in parameter in call",                   /* 函数调用参数类型不符合 */
    //"unreachable code",                                     /* return 后有语句 code after return */
    "undefined reference to 'main'"                         /* 缺少main函数 */
};

char fatmsg[20][100] = {
    "could not find input file",                            /* 没有找到输入文件 */
    "error writing output file",                            /* 写输出文件错误 */
    //"incorrect command line argument",                      /* 错误命令行参数 */
    "out of table",                                         /* 符号表溢出 */
    //"out of line buffer",                                   /* 行缓冲区溢出 */
    "out of memory"                                         /* 内存申请失败 */
};

char warningmsg[10][100] = {
    "overflow in implicit constant conversion",             /* 给字符变量赋值超过127 */
    "'return' with a value, in function returning void"     /* 函数额外return 值 */
};

struct Set Errset;

void error(int n){
    s_insert(&Errset, n);
    if(n == ep_main){
        printf("error: ");
        printf("%s\n", errmsg[n]);
        return;
    }
    printf("%d:%d ", line, np - 1);
    printf("error: ");
    printf("%s\n", errmsg[n]);
    printf("%s", linebuf);
    int i;
    for(i = 1; i < errpos; i++){
        printf(" ");
    }
    printf("^");
    while(errpos + 1 < cc){
        printf("~");
        errpos++;
    }
    printf("\n");
}

void fatal(int n){
    printf("fatal: ");
    printf("%s\n", fatmsg[n]);
    exit(0);
}

void warning(int n){
    printf("%d:%d ", line, np - 1);
    printf("warning: ");
    printf("%s\n", warningmsg[n]);
    printf("%s", linebuf);
    int i;
    for(i = 1; i < errpos; i++){
        printf(" ");
    }
    printf("^");
    while(errpos + 1 < cc){
        printf("~");
        errpos++;
    }
    printf("\n");
}