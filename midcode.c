#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"
#include "table.h"
#include "midcode.h"


struct FCode *mCode[CODELMX];
int mcptr = 0;

struct FCode *mOptCode[CODELMX];
int mOptcptr = 0;

void calForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    if(b == 0 || res == 0){
        printf("cal form log failed\n");
        return;
    }
    if(a->kind == constant && b->kind == constant){
        if(PRINTMIDCODE) printf("%s = %d %s %d\n", res->name, a->adr, strMap[op], b->adr);
        if(isGenMid) fprintf(midfp, "%s = %d %s %d\n", res->name, a->adr, strMap[op], b->adr);
    }else if(a->kind != constant && b->kind == constant){
        if(PRINTMIDCODE) printf("%s = %s %s %d\n", res->name, a->name, strMap[op], b->adr);
        if(isGenMid) fprintf(midfp, "%s = %s %s %d\n", res->name, a->name, strMap[op], b->adr);
    }else if(a->kind == constant && b->kind != constant){
        if(PRINTMIDCODE) printf("%s = %d %s %s\n", res->name, a->adr, strMap[op], b->name);
        if(isGenMid) fprintf(midfp, "%s = %d %s %s\n", res->name, a->adr, strMap[op], b->name);
    }else{
        if(PRINTMIDCODE) printf("%s = %s %s %s\n", res->name, a->name, strMap[op], b->name);
        if(isGenMid) fprintf(midfp, "%s = %s %s %s\n", res->name, a->name, strMap[op], b->name);
    }
}

void arrForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    if(a == 0 || b == 0 || res == 0){
        printf("arr form log failed\n");
        return;
    }
    if(type == mid_stAry){
        if(b->kind == constant && a->kind == constant){
            if(PRINTMIDCODE) printf("%s[%d] = %d\n", res->name, b->adr, a->adr);
            if(isGenMid) fprintf(midfp, "%s[%d] = %d\n", res->name, b->adr, a->adr);
        }
        else if(b->kind != constant && a->kind == constant){
            if(PRINTMIDCODE) printf("%s[%s] = %d\n", res->name, b->name, a->adr);
            if(isGenMid) fprintf(midfp, "%s[%s] = %d\n", res->name, b->name, a->adr);
        }
        else if(b->kind == constant && a->kind != constant){
            if(PRINTMIDCODE) printf("%s[%d] = %s\n", res->name, b->adr, a->name);
            if(isGenMid) fprintf(midfp, "%s[%d] = %s\n", res->name, b->adr, a->name);
        }else{
            if(PRINTMIDCODE) printf("%s[%s] = %s\n", res->name, b->name, a->name);
            if(isGenMid) fprintf(midfp, "%s[%s] = %s\n", res->name, b->name, a->name);
        }
    }else{
        if(b->kind == constant){
            if(PRINTMIDCODE) printf("%s = %s[%d]\n", res->name, a->name, b->adr);
            if(isGenMid) fprintf(midfp, "%s = %s[%d]\n", res->name, a->name, b->adr);
        }
        else{
            if(PRINTMIDCODE) printf("%s = %s[%s]\n", res->name, a->name, b->name);
            if(isGenMid) fprintf(midfp, "%s = %s[%s]\n", res->name, a->name, b->name);
        }
    }
}

void branchForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int type;
    int op;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    if(res == 0){
        printf("branch form log failed\n");
        return;
    }
    switch(type){
        case mid_goFalse:{
            if(b == 0){
                if(a->kind == constant){
                    if(PRINTMIDCODE) printf("ifFalse %d goto (label_%s)%d\n", a->adr, res->name, res->adr);
                    if(isGenMid) fprintf(midfp, "ifFalse %d goto (label_%s)%d\n", a->adr, res->name, res->adr);
                }else{
                    if(PRINTMIDCODE) printf("ifFalse %s goto (label_%s)%d\n", a->name, res->name, res->adr);
                    if(isGenMid) fprintf(midfp, "ifFalse %s goto (label_%s)%d\n", a->name, res->name, res->adr);
                }
            }else if(a->kind == constant && b->kind == constant){
                if(PRINTMIDCODE) printf("ifFalse %d %s %d goto (label_%s)%d\n", a->adr, strMap[op], b->adr, res->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifFalse %d %s %d goto (label_%s)%d\n", a->adr, strMap[op], b->adr, res->name, res->adr);
            }else if(a->kind != constant && b->kind == constant){
                if(PRINTMIDCODE) printf("ifFalse %s %s %d goto (label_%s)%d\n", a->name, strMap[op], b->adr, res->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifFalse %s %s %d goto (label_%s)%d\n", a->name, strMap[op], b->adr, res->name, res->adr);
            }else if(a->kind == constant && b->kind != constant){
                if(PRINTMIDCODE) printf("ifFalse %d %s %s goto (label_%s)%d\n", a->adr, strMap[op], b->name, res->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifFalse %d %s %s goto (label_%s)%d\n", a->adr, strMap[op], b->name, res->name, res->adr);
            }else{
                if(PRINTMIDCODE) printf("ifFalse %s %s %s goto (label_%s)%d\n", a->name, strMap[op], b->name, res->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifFalse %s %s %s goto (label_%s)%d\n", a->name, strMap[op], b->name, res->name, res->adr);
            }
            break;
        }
        case mid_goTrue:{
            if(a->kind == constant && b->kind == constant){
                if(PRINTMIDCODE) printf("ifTrue %d %s %d goto %d\n", a->adr, strMap[op], b->adr, res->adr);
                if(isGenMid) fprintf(midfp, "ifTrue %d %s %d goto %d\n", a->adr, strMap[op], b->adr, res->adr);
            }else if(a->kind != constant && b->kind == constant){
                if(PRINTMIDCODE) printf("ifTrue %s %s %d goto %d\n", a->name, strMap[op], b->adr, res->adr);
                if(isGenMid) fprintf(midfp, "ifTrue %s %s %d goto %d\n", a->name, strMap[op], b->adr, res->adr);
            }else if(a->kind == constant && b->kind != constant){
                if(PRINTMIDCODE) printf("ifTrue %d %s %s goto %d\n", a->adr, strMap[op], b->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifTrue %d %s %s goto %d\n", a->adr, strMap[op], b->name, res->adr);
            }else{
                if(PRINTMIDCODE) printf("ifTrue %s %s %s goto %d\n", a->name, strMap[op], b->name, res->adr);
                if(isGenMid) fprintf(midfp, "ifTrue %s %s %s goto %d\n", a->name, strMap[op], b->name, res->adr);
            }
            break;
        }
        case mid_goto:{
            if(res->kind == constant){
                if(PRINTMIDCODE) printf("goto %d\n", res->adr);
                if(isGenMid) fprintf(midfp, "goto %d\n", res->adr);
            }else{
                if(PRINTMIDCODE) printf("goto %d\n", res->ref);
                if(isGenMid) fprintf(midfp, "goto %d\n", res->ref);
            }
            break;
        }
        default: printf("code confilict\n");
    }
}

void cpyForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    if(a == 0 || res == 0){
        printf("cpy form log failed\n");
        return;
    }
    if(a->kind == constant){
        if(PRINTMIDCODE) printf("%s = %d\n", res->name, a->adr);
        if(isGenMid) fprintf(midfp, "%s = %d\n", res->name, a->adr);
    }else{
        if(PRINTMIDCODE) printf("%s = %s\n", res->name, a->name);
        if(isGenMid) fprintf(midfp, "%s = %s\n", res->name, a->name);
    }
}

void labelForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    if(res == 0){
        printf("label form log failed\n");
        return;
    }
    if(PRINTMIDCODE) printf("label_%s\n", res->name);
    if(isGenMid) fprintf(midfp, "label_%s\n", res->name);
}

void callForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int count;
    int i;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    if(a == 0){
        printf("call form log failed\n");
        return;
    }
    if(res != 0){
        if(PRINTMIDCODE) printf("%s = %s(", res->name, a->name);
        if(isGenMid) fprintf(midfp, "%s = %s(", res->name, a->name);
    }
    else{
        if(PRINTMIDCODE) printf("%s(", a->name);
        if(isGenMid) fprintf(midfp, "%s(", a->name);
    }
    for(i = 0; i < count; i++){
        if(PRINTMIDCODE) printf(i == 0 ? "%s" : ", %s",  c->paraList[i]->name);
        if(isGenMid) fprintf(midfp, i == 0 ? "%s" : ", %s",  c->paraList[i]->name);
    }
    if(PRINTMIDCODE) printf(")\n");
    if(isGenMid) fprintf(midfp, ")\n");
}

void retForm(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    if(res != 0){
        if(PRINTMIDCODE) printf("ret %s\n", res->name);
        if(isGenMid) fprintf(midfp, "ret %s\n", res->name);
    }else{
        if(PRINTMIDCODE) printf("ret\n");
        if(isGenMid) fprintf(midfp, "ret\n");
    }
}

void outputMcode(){
    int i;
    int type;
    struct FCode *c;

    for(i=0; i<mcptr; i++){
        type = mCode[i]->type;
        c = mCode[i];
        if(PRINTMIDCODE) printf("(%d) ", i);
        if(isGenMid) fprintf(midfp, "(%d) ", i);
        if(type == mid_cal){
            calForm(c);
        }else if(type == mid_stAry || type == mid_ldAry){
            arrForm(c);
        }else if(type == mid_goFalse || type == mid_goTrue ||type == mid_goto){
            branchForm(c);
        }else if(type == mid_cpy){
            cpyForm(c);
        }else if(type == mid_label){
            labelForm(c);
        }else if(type == mid_call || type == mid_printf || type == mid_scanf){
            callForm(c);
        }else if(type == mid_ret){
            retForm(c);
        }
    }
}

void outputOPMcode(){
    int i;
    int type;
    struct FCode *c;

    for(i=0; i<mOptcptr; i++){
        type = mOptCode[i]->type;
        c = mOptCode[i];
        printf("(%d) ", i);
        if(type == mid_cal){
            calForm(c);
        }else if(type == mid_stAry || type == mid_ldAry){
            arrForm(c);
        }else if(type == mid_goFalse || type == mid_goTrue ||type == mid_goto){
            branchForm(c);
        }else if(type == mid_cpy){
            cpyForm(c);
        }else if(type == mid_label){
            labelForm(c);
        }else if(type == mid_call || type == mid_printf || type == mid_scanf){
            callForm(c);
        }else if(type == mid_ret){
            retForm(c);
        }
    }
}

void copyOPMcode(){
    int i;
    
    for(i = 0; i < mcptr; i++){
        mCode[i] = 0;
    }

    mcptr = 0;

    for(i = 0; i < mOptcptr; i++){
        mCode[i] = mOptCode[i];
        mCode[i]->bbsign = 0;
        if(mCode[i]->type == mid_label){
            mCode[i]->res->adr = i;
        }
        mcptr++;
    }
}

void emit(int type, int op, struct SymItem *a,  struct SymItem *b, struct SymItem *res){
    struct FCode *c;

    c = (struct FCode *)malloc(sizeof(struct FCode));
    c->type = type;
    c->op = op;
    c->a = a;
    c->b = b;
    c->res = res;
    c->count = 0;
    c->paraList[0] = 0;
    c->bbsign = 0;
    c->belong2BB = -1;
    c->isOld = 0;
    
    
    mCode[mcptr++] = c;
    
    /*
    if(PRINTMIDCODE){
        printf("(%d) ", mcptr-1);
        if(type == mid_cal){
            calForm(c);
        }else if(type == mid_stAry || type == mid_ldAry){
            arrForm(c);
        }else if(type == mid_goFalse || type == mid_goTrue ||type == mid_goto){
            branchForm(c);
        }else if(type == mid_cpy){
            cpyForm(c);
        }else if(type == mid_label){
            labelForm(c);
        }else if(type == mid_call || type == mid_printf || type == mid_scanf){
            callForm(c);
        }else if(type == mid_ret){
            retForm(c);
        }
    }
    */
}