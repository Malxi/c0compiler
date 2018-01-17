#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"
#include "table.h"
#include "semantic.h"
#include "midcode.h"

char tmpName[30] = "t";
int tmpCount = 0;
int curfunc = -1;

struct SymItem *constPool[CONSTPOOLSIZE] = {0};
int constPtr = 0;

int isInConstPool(int type, int adr){
    int i;
    for(i = 0; i < constPtr; i++){
        if(constPool[i]->type == type && constPool[i]->adr == adr){
            return i;
        }
    }
    return -1;
}

int kindCheck(struct SymItem *a, struct SymItem *b){
    if(a->kind == constant && b->kind == constant) return constant;
    else return variable;
}

int typeCheck(struct SymItem *a, struct SymItem *b){
    if(a->type == ints && b->type == ints) return ints;
    if(a->type == ints && b->type == chars) return ints;
    if(a->type == chars && b->type == ints) return ints;
    if(a->type == chars && b->type == chars) return chars;
}

struct SymItem *newTmpItem(int kind, int type, int adr){
    struct SymItem *tmp;
    char *name;
    char num[20] = {0};
    //int i;
    int len;
    tmp = (struct SymItem *)malloc(sizeof(struct SymItem));
    if(tmp == NULL){
        fatal(3);
    }
    len = strlen(tmpName) + 1;
    name = (char *)malloc(len);
    strcpy(name, tmpName);
    name[len] = '\0';
    sprintf(num, "%d", tmpCount);
    strcat(name, num);
    if(tmp == NULL){
        printf("Error\n");
    }
    tmp->name = name;
    tmp->kind = kind;
    tmp->type = type;
    tmp->lev = curlev;
    tmp->adr = adr;
    tmp->ref = 0;
    tmp->link = 0;
    tmp->size = 1;
    tmp->isTmp = 1;
    tmp->num = varId++;
    tmp->end = 0;
    tmp->hasRet = 0;
    tmp->count = 0;
    tmp->reg = -1;
    tmp->loop = -1;
    tmp->dirty = 0;
    tmp->isModified = 0;
    tmp->creg = -1;
    tmp->isMain = 0;
    tmpCount++;
    return tmp;
}

void skip(struct Set *fsys, int err){
    error(err);
    while(!s_has(fsys, sy)){
        nextsy();
    }
}

void test(struct Set *s1, struct Set *s2, int err){
    struct Set *tmp;
    s_clone(&tmp, s1);
    s_plus(tmp, s2);
    if(!s_has(s1, sy)){
        skip(tmp, err);
    }
    free(tmp);
}

void testsemicolon(struct Set *s, int err){
    //struct Set *follow;
    if(sy == semicolon) nextsy();
    else{
        error(ep_cs);
        if(sy == comma || sy == colon) nextsy();
    }
    /*
    s_clone(&follow, s);
    s_init(follow);
    s_insert(follow, flo);
    */
    test(s, s, err);
    //free(follow);
}

struct SymItem *arrSelector(struct Set *fsys){
    /* ident[<expresion>] */
    /* 
        @sy = lbrack
    */
    struct SymItem *offset = NULL;
    if(sy == lbrack){
        nextsy();
        /* need to parse experssion */
        if(sy == rbrack){
            error(ep_expr);
        }
        offset = expressionParse(fsys);
        /*check final rbrack */
        if(sy == rbrack){
            nextsy();
        }else{
            error(arr_rbract);
        }
    }else{
        error(arr_lbract);
    }
    if(offset == NULL){
        offset = newTmpItem(constant, ints, 0);
    }
    return offset;
}

struct SymItem *callParse(struct Set *fsys, int i){
    /* <ident>[(<expression>{,<expression>})] */
    /*
        @sy = ident (kind = function)
    */
    struct SymItem *func;
    struct SymItem *para;
    struct SymItem *res;
    struct FCode *callcode;
    int cp = i + 1;
    int typ;
    int count = 0;
    int extraFlag = 1;

    if(LOG) printf("log: function call statement begin\n");
    
    func = tab[i];
    emit(mid_call, 0, func, 0, 0);
    callcode = mCode[mcptr - 1];
    mcptr--;
    /* next symbol is lparent or semicolon */
    //nextsy(); /* read nextsy to help error handel */
    /* function has parameters */
    if(func->paras > 0){
        if(sy == lparent){
            /* next symbol is expression */
            nextsy();
            while(s_has(&factbegsys, sy)){
                para = expressionParse(fsys);
                /* parameters match */
                if(tab[cp]->kind != parameter){
                    if(extraFlag){
                        error(para_extra);
                        extraFlag = 0;
                    }
                }
                typ = typeCheck(tab[cp], para);
                if(tab[cp]->type == typ){
                    //res = newTmpItem(parameter, typ, 0);
                    //emit(para, res);
                    //emit(mid_cpy, 0, para, 0, res);
                    callcode->paraList[count++] = para;
                }else{
                    error(para_mismatch);
                    //res = newTmpItem(parameter, typ, 0);
                    //emit(para, res);
                    //emit(mid_cpy, 0, para, 0, res);
                    callcode->paraList[count++] = para;
                }
                cp++;
                /* next is comma or rparant */
                if(sy == comma){
                    nextsy();
                }
            }
            /* rparant */
            if(sy == rparent){
                nextsy();
            }else{
                error(ep_rparent);
            }
        }else{
            error(ep_lparent);
        }
    }
    /* function has no parameters */
    if(func->paras == 0){
        /* next symbol is semicolon */
        //nextsy();
    }
    callcode->count = count;
    if(cp - i < func->paras - 1){
        error(para_few);
    }
    //emit(mid_goto, 0, 0, 0, func);
    res = newTmpItem(variable, func->type, 0);
    //emit(func, res);
    //emit(mid_cpy, 0, func, 0, res);
    callcode->res = res;
    mCode[mcptr++] = callcode;
    if(LOG) printf("log: function call statement end\n");
    return res;
}

struct SymItem *factorParse(struct Set *fsys){
    /* <ident> | <ident>[<expression>] | (<expression>) | intcon | charcon | <retFunctionCall> */
    /*
        sy = factbegsys
    */
    struct SymItem *res, *a, *b, *offset, *tmp;
    int op;
    struct Set *ftmp;
    int i, typ;
    int sig = 1;
    int r;

    res = 0;

    test(&factbegsys, fsys, expr_syntax);
    s_clone(&ftmp, &factbegsys);
    /*
    if(s_has(&factbegsys, sy)){
        if(sy == ident){
            i = locSymbol(id);
            // next symbol is 
            res = tab[i];
            switch(tab[i]->kind){
                case variable : {
                    res = tab[i];
                    nextsy(); 
                    break;
                }
                case array : {
                    nextsy();
                    offset = arrSelector(ftmp);
                    tmp = newTmpItem(variable, res->type, 0);
                    //emit_2(plus, res, offset, tmp);
                    emit(mid_ldAry, 0, res, offset, tmp);
                    res = tmp;
                    break;
                }
                case constant : {
                    res= tab[i];
                    nextsy();
                    break;
                }
                case function: {
                    if(res->type == voids){
                        error(func_nret);
                    }
                    res = callParse(fsys, i);
                    break;
                }
                case parameter: {
                    res = tab[i]; 
                    nextsy();
                    break;
                }
                default: {
                    error(id_ndel);
                    nextsy();
                    //test(&factbegsys, fsys, id_ndel);
                }
            }
        }else if(sy == charcon || sy == intcon){
            typ = sy == charcon ? chars : ints;
            tmp = newTmpItem(constant, typ, inum);
            res = tmp;
            nextsy();
        }else if(sy == plus || sy == minus){
            sig = sy == plus? 1 : -1;
            nextsy();
            if(sy == intcon){
                tmp = newTmpItem(constant, ints, sig*inum);
                res = tmp;
                nextsy();
            }else{
                error(expr_syntax);
            }
        }else if(sy == lparent){
            nextsy();
            s_insert(ftmp, rparent);
            res = expressionParse(ftmp);
            if(sy == rparent){
                nextsy();
            }else{
                error(ep_rparent);
            }
        }else{
            test(&factbegsys, fsys, expr_syntax);
            return factorParse(fsys);
        }
        //test(fsys, factfsys, expr_syntax);
    }else{
        test(&factbegsys, fsys, expr_syntax);
        return factorParse(fsys);
    }
    */
    while(s_has(&factbegsys, sy)){
        if(sy == ident){
            i = locSymbol(id);
            // next symbol is 
            res = tab[i];
            switch(tab[i]->kind){
                case variable : {
                    nextsy();
                    return tab[i];
                    break;
                }
                case array : {
                    nextsy();
                    offset = arrSelector(ftmp);
                    if(offset->kind == constant && (offset->adr >= res->size || offset->adr < 0)){
                        error(arr_index);
                    }
                    tmp = newTmpItem(variable, res->type, 0);
                    //emit_2(plus, res, offset, tmp);
                    emit(mid_ldAry, 0, res, offset, tmp);
                    return tmp;
                    break;
                }
                case constant : {
                    nextsy();
                    return tab[i];
                    break;
                }
                case function: {
                    if(res->type == voids){
                        error(func_nret);
                    }
                    nextsy();
                    return callParse(fsys, i);
                    break;
                }
                case parameter: {
                    nextsy();
                    return tab[i];
                    break;
                }
                default: {
                    error(id_ndel);
                    nextsy();
                    return res;
                    //test(&factbegsys, fsys, id_ndel);
                }
            }
        }else if(sy == charcon || sy == intcon){
            typ = sy == charcon ? chars : ints;
            r = isInConstPool(typ, inum);
            if(r == -1){
                tmp = newTmpItem(constant, typ, inum);
                constPool[constPtr++] = tmp;
            }else{
                tmp = constPool[r];
            }
            nextsy();
            return tmp;
        }else if(sy == plus || sy == minus){
            sig = sy == plus? 1 : -1;
            nextsy();
            if(sy == intcon){
                tmp = newTmpItem(constant, ints, sig*inum);
                nextsy();
                return tmp;
            }else{
                error(expr_syntax);
            }
        }else if(sy == lparent){
            nextsy();
            s_insert(ftmp, rparent);
            res = expressionParse(ftmp);
            if(sy == rparent){
                nextsy();
            }else{
                error(ep_rparent);
            }
            return res;
        }else{
            nextsy();
        }
        test(&factbegsys, fsys, expr_syntax);
    }
    free(ftmp);
    if(res == 0) res = newTmpItem(constant, ints, 0);
    return res;
}

struct SymItem *termParse(struct Set *fsys){
    /* <factor>{*<factor> | /<factor>} */
    struct SymItem *res, *a, *b, *offset, *tmp;
    int op;
    struct Set *ftmp;
    int typ, knd;

    ftmp = s_newSet(fsys);
    //s_insert(ftmp, times);
    //s_insert(ftmp, idiv);

    a = factorParse(ftmp);
    while(sy == times || sy == idiv){
        op = sy;
        nextsy();
        b = factorParse(ftmp);
        /* type check*/
        typ = typeCheck(a, b);
        knd = kindCheck(a, b);
        res = newTmpItem(variable, typ, 0);
        //emit_2(op, a, b, res);
        if(b->kind == constant && op == idiv && b->adr == 0){
            error(div_zero);
        }
        emit(mid_cal, op, a, b, res);
        a = res;
    }
    res = a;
    free(ftmp);
    return res;
}

struct SymItem *expressionParse(struct Set *fsys){
    /* [+-]<term>{+<term> | -<term>}*/
    /*
        sy = factbegsys
    */
    struct SymItem *res, *a, *b, *offset, *tmp;
    int op;
    struct Set *ftmp;
    int typ, knd;

    if(LOG) printf("log: expression statement begin\n");

    ftmp = s_newSet(fsys);
    //s_insert(ftmp, plus);
    //s_insert(ftmp, minus);
    
    if(sy == plus || sy == minus){
        op = sy;
        nextsy();
        a = termParse(ftmp);
        tmp = newTmpItem(variable, a->type, 0);
        b = newTmpItem(constant, a->type, 0);
        //emit_1(op, a, tmp);
        emit(mid_cal, op, b, a, tmp);
        a = tmp;
    }else{
        a = termParse(ftmp);
    }
    res = a;
    while(sy == plus || sy == minus){
        op = sy;
        nextsy();
        b = termParse(ftmp);
        typ = typeCheck(a, b);
        knd = kindCheck(a, b);
        res = newTmpItem(variable, typ, 0);
        //emit_2(op, a, b, res);
        emit(mid_cal, op, a, b, res);
        a = res;
    }
    free(ftmp);
    if(LOG) printf("log: expression statement end\n");
    return res;
}

void assignmentParse(struct Set *fsys, int i){
    /* ident['['intcon']'] = expression; */
    /*
        @sy = ident
    */
    struct SymItem *res, *a, *b, *offset, *tmp;
    int op, kind;
    struct Set *ftmp;
    struct SymItem *assign;

    if(LOG) printf("log: assignment statement begin\n");

    kind = tab[i]->kind;
    res = tab[i];
    ftmp = s_newSet(0);
    assign = tab[i];
    
    /* next is [ or = */
    /* read next symbol to help error handel */
    //nextsy();
    s_insert(ftmp, lbrack);
    s_insert(ftmp, becomes);
    test(ftmp, fsys, ep_becomes);

    /* deal with array */
    if(kind == array){
        offset = arrSelector(fsys);
        if(offset->kind == constant && (offset->adr >= assign->size || offset->adr < 0)){
            error(arr_index);
        }
    }

    /* deal with variable */
    if(sy == becomes){
        nextsy();
        /* need to parse expression */
        a = expressionParse(fsys);
        if(kind == array){
            tmp = newTmpItem(variable, ints, 0);
            //emit_2(plus, res, offset, tmp);
            //emit(a, tmp);
            /* check type only when assign type is char */
            if(assign->type == chars && a->type == ints){
                if(a->kind == constant && (a->adr >= 128 || a->adr < 0)){
                    warning(of_conversion);
                    a->adr = a->adr & 0xff;
                }else if(a->kind != constant){
                    warning(of_conversion);
                    a->adr = a->adr & 0xff;
                }
            }
            emit(mid_stAry, 0, a, offset, res);
        }else{
            //emit(a, res);
            if(assign->type == chars && a->type == ints){
                if(a->kind == constant && (a->adr >= 128 || a->adr < 0)){
                    warning(of_conversion);
                        a->adr = a->adr & 0xff;
                }else if(a->kind != constant){
                    warning(of_conversion);
                    a->adr = a->adr & 0xff;
                }
            }
            emit(mid_cpy, 0, a, 0, res);
        }
    }else{
        error(expr_syntax);
    }
    free(ftmp);
    if(LOG) printf("log: assignment statement end\n");
}

void identInfer(struct Set *fsys){
    /* <assignment>|<functioncall>*/
    /*
        @sy = ident
        @error is ndel, need to return
    */
    int i;
    struct Set *tmpfsys;
    tmpfsys = s_newSet(&statbegsys);
    s_insert(tmpfsys, rbrace);

    i = locSymbol(id);
    if(i > 0){
        switch(tab[i]->kind){
            case constant: {
                error(const_assign);
                nextsy();
                assignmentParse(fsys, i);
                break;
            }
            case variable: {
                nextsy();
                assignmentParse(fsys, i);
                break;
            }
            case parameter: {
                nextsy();
                assignmentParse(fsys, i);
                break;
            }
            case array: {
                nextsy();
                assignmentParse(fsys, i);
                break;
            }
            case function: {
                nextsy();
                callParse(fsys, i);
                break;
            }
        }
        testsemicolon(tmpfsys, ep_cs);
    }else{
        error(id_ndel);
        nextsy();
        if(sy == becomes){
            assignmentParse(fsys, i);
            testsemicolon(tmpfsys, ep_cs);
        }else {
            test(fsys, tmpfsys, ep_state);
        }
    }
    free(tmpfsys);
}

struct SymItem *caseParse(struct Set *fsys, struct SymItem *expr, struct SymItem *lb_e){
    /* case 1: <constant> @label <statement> @goto*/
    /*
        @sy = casesy
        @ret label
    */
    struct SymItem *con, *lb;
    int sign = 1;

    if(LOG) printf("log: case statement begin\n");
    lb = newTmpItem(label, ints, 0);
    /* next symbol is intcon or charcon */
    nextsy();
    if(sy == plus || sy == minus){
        sign = sy == plus ? 1 : -1;
        nextsy();
        if(sy == charcon){
            error(ep_const);
        }
    }
    if(sy == charcon || sy == intcon){
        con = newTmpItem(constant, ints, sign*inum);
        //emit_branch(eql, expr, con, lb);
        emit(mid_goFalse, eql, expr,  con, lb);
        nextsy();
    }else{
        error(case_con);
    }
    /* symbol is colon */
    if(sy == colon){
        nextsy();
    }else{
        error(case_colon);
    }
    /* statement parse */
    if(s_has(&statbegsys, sy)){
        statementParse(fsys);
    }else{
        error(case_stat);
    }
    //emit_jump(lb_e);
    emit(mid_goto, 0, 0, 0, lb_e);
    if(LOG) printf("log: case statement end\n");
    return lb;
}

void switchstatementParse(struct Set *fsys){
    /* switch(<expression){<caseList>} */
    /*
        @sy = switchsy
    */
    struct SymItem *expr, *con, *lb, *lb_e;
    int op;
    struct Set *ftmp, *factfsys;

    if(LOG) printf("log: switch statement begin\n");

    lb_e = newTmpItem(constant, ints, 0);
    /* next symbol is lparent */
    nextsy();
    if(sy == lparent){
        nextsy();
        expr = expressionParse(fsys);
        if(sy == rparent){
            nextsy();
        }else{
            error(ep_rparent);
        }
    }else{
        error(ep_lparent);
    }
    /* case list */
    if(sy == lbrace){
        /* next symbol is case */
        nextsy();
        /* one case */
        if(sy == casesy){
            lb = caseParse(fsys, expr, lb_e);
            lb->adr = mcptr;
            //if(DEBUG) printf("swich_labe_%s\n", lb->name);
            emit(mid_label, 0, 0, 0, lb);
        }else{
            error(ep_cs);
            /* need to change this */
        }
        while(sy == casesy){
            lb = caseParse(fsys, expr, lb_e);
            lb->adr = mcptr;
            //if(DEBUG) printf("swich_labe_%s\n", lb->name);
            emit(mid_label, 0, 0, 0, lb);
        }
        /* default parse */
        //if(DEBUG) printf("swich_labe_%s\n", lb_e->name);
        if(sy == defaultsy){
            nextsy();
            /* symbol is colon */
            if(sy == colon){
                nextsy();
            }else{
                error(default_colon);
            }
            if(s_has(&statbegsys, sy)){
                statementParse(fsys);
            }else{
                error(ep_state);
            }
        }
        lb_e->adr = mcptr;
        emit(mid_label, 0, 0, 0, lb_e);
        if(sy == rbrace){
            nextsy();
        }else{
            error(ep_rbrace);
        }
    }else{
        error(ep_lbrace);
    }
    if(LOG) printf("log: switch statement end\n");
}

struct SymItem *conditionParse(struct Set *fsys){
    /* '('<expression>[<logic op><expression>]')' */
    /*
        @sy = lparent
    */
    struct SymItem *res, *lb, *a, *b;
    int op = -1;

    a = 0;
    b = 0;

    if(sy == lparent){
        /* next symbol is factbegsys */
        nextsy();
        if(sy == rparent){
            error(ep_expr);
            nextsy();
        }else{
            a = expressionParse(fsys);
            /* next symbol is logic or rparent*/
            if(sy == geq || sy == neq || sy == gtr || sy == eql || sy == lss || sy == leq){
                op = sy;
                nextsy();
                b = expressionParse(fsys);
            }
            if(sy == rparent){
                nextsy();
            }else{
                error(ep_rparent);
            }
        }
    }else{
        error(ep_lparent);
    }
    /* condition code generate */
    lb = newTmpItem(constant, label, 0);
    if(op != -1){
        //emit_branch(op, a, b, lb);
        if(a == 0) a = newTmpItem(constant, ints, 0);
        if(b == 0) b = newTmpItem(constant, ints, 0);
        emit(mid_goFalse, op, a, b, lb);
    }else{
        //res = newTmpItem(constant, ints, 0);
        //emit_branch(neq, a, res, lb);
        if(a == 0) a = newTmpItem(constant, ints, 0);
        emit(mid_goFalse, neq, a, 0, lb);
    }
    return lb;
}

void whilestatemnetParse(struct Set *fsys){
    /* while(<condition>)<statement> */
    /* 
        @sy = whilesy
        @check rbrace or semicolon
    */
    struct SymItem *res, *a, *b, *offset, *lb_1, *lb_2;
    int op;
    struct Set *ftmp, *factfsys;

    if(LOG) printf("log: while statement begin\n");

    lb_1 = newTmpItem(constant, label, mcptr);
    lb_1->loop = 1;
    //if(DEBUG) printf("while_label_%s\n", lb_1->name);
    emit(mid_label, 0, 0, 0, lb_1);
    /* next symbol is lparent */
    nextsy();
    /* condition */
    lb_2 = conditionParse(fsys);
    /* next parse a statement */
    if(s_has(&statbegsys, sy)){
        statementParse(fsys);
    }else{
        error(ep_state);
    }
    //emit_jump(lb_1);
    emit(mid_goto, 0, 0, 0, lb_1);
    lb_2->adr = mcptr;
    //if(DEBUG) printf("while_label_%s\n", lb_2->name);
    emit(mid_label, 0, 0, 0, lb_2);
    if(LOG) printf("log: while statement end\n");
}

void ifstatementParse(struct Set *fsys){
    /* if(<condition>)<statement>else<statement> */
    /* 
        @sy = ifsy
        @check rbrace or semicolon
    */
    struct SymItem *res, *a, *b, *offset, *lb_1, *lb_2;
    int op;
    struct Set *ftmp, *factfsys;
    int hasElse = 0;

    if(LOG) printf("log: if statement begin\n");

    /* next symbol is lparent */
    nextsy();
    /* condition */
    lb_1 = conditionParse(fsys);
    /* next parse a statement */
    if(s_has(&statbegsys, sy)){
        statementParse(fsys);
    }else{
        error(ep_state);
    }
    /* next symbol is else */
    if(sy == elsesy){
        nextsy();
        hasElse = 1;
    }else{
        error(if_else);
        hasElse = 0;
    }
    lb_2 = newTmpItem(constant, label, 0);
    //emit_jump(lb_2);
    emit(mid_goto, 0, 0, 0, lb_2);
    lb_1->adr = mcptr;
    //if(DEBUG) printf("if_labe_%s\n", lb_1->name);
    emit(mid_label, 0, 0, 0, lb_1);
    /* next parse a statement */
    if(hasElse == 0){
        //do nothing
    }else if(s_has(&statbegsys, sy)){
        statementParse(fsys);
    }else{
        error(ep_state);
    }
    lb_2->adr = mcptr;
    //if(DEBUG) printf("if_labe_%s\n", lb_2->name);
    emit(mid_label, 0, 0, 0, lb_2);
    if(LOG) printf("log: if statement end\n");
}

void returnstatementParse(struct Set *fsys){
    /* return [(<expression>)] */
    /*
        @sy = returnsy
        @check return value
    */
    struct SymItem *res;
    struct SymItem *ra;

    struct Set *tmp;
    tmp = s_newSet(fsys);
    s_insert(tmp, rparent);
    //ra = newTmpItem(variable, ints, 0);

    if(LOG) printf("log: return statement begin\n");
    /* next symbol is lparent */
    nextsy();
    if(sy == lparent){
        nextsy();
        res = expressionParse(tmp);
        /* we function type is chars, we need to convert type */
        switch(tab[curfunc]->type){
            case voids: error(func_nret);break;
            case ints:{
                break;
            }
            case chars:{
                if(res->type == ints){
                    if(res->kind == constant && (res->adr >= 128 || res->adr < 0)){
                        warning(of_conversion);
                        res->adr = res->adr & 0xff;
                    }else if(res->kind != constant){
                        warning(of_conversion);
                        res->adr = res->adr & 0xff;
                    }
                }
            }
        }
        //emit(res, tab[curfunc]);
        //emit(mid_cpy, 0, res, 0, tab[curfunc]);
        if(sy == rparent){
            nextsy();
        }else{
            error(ep_rparent);
        }
    }else if(sy == semicolon){
        /* return */
        res = 0;
    }
    else{
        error(ep_lparent);
        res = 0;
    }
    free(tmp);
    emit(mid_ret, 0, 0, 0, res);
    tab[curfunc]->hasRet = 1;
    if(LOG) printf("log: return statement end\n");
}

void scanfstatementParse(struct Set *fsys){
    /* scanf(<ident>{,ident}) */
    /*
        @sy = printfsy
        @scanf ident
    */
    int i = 0;
    struct SymItem *res;
    int count = 0;
    struct FCode *callcode;

    if(LOG) printf("log: scanf statement begin\n");
    /* new function call code */
    res = newTmpItem(function, voids, 0);
    res->name = getName();
    emit(mid_scanf, 0, res, 0, 0);
    callcode = mCode[mcptr - 1];
    mcptr--;
    /* next symbol is lparent */
    nextsy();
    if(sy == lparent){
        nextsy();
        if(sy == ident){
            i = locSymbol(id);
            if(i == 0){
                error(id_ndel);
            }
            //para[count++] = tab[i];
            callcode->paraList[count++] = tab[i];
            nextsy();
        }else{
            error(ep_ident);
        }
        if(sy == comma){
            nextsy();
        }
        while(sy == ident){
            i = locSymbol(id);
            if(i == 0){
                error(id_ndel);
            }
            //para[count++] = tab[i];
            callcode->paraList[count++] = tab[i];
            nextsy();
            if(sy == comma){
                nextsy();
            }
        }
        if(sy == rparent){
            nextsy();
        }else{
            error(ep_rparent);
        }
    }else{
        error(ep_lparent);
    }
    callcode->count = count;
    mCode[mcptr++] = callcode;
    if(LOG) printf("log: scanf statement end\n");
}

void printfstatementParse(struct Set *fsys){
    /* printf(strcon,<ident>) | printf(strcon) | printf(<expression>) */
    /*
        @sy = printfsy
        @check ident
    */
    int i = 0;
    struct SymItem *res;
    struct SymItem *para[100];
    char *str;
    int count = 0;
    struct FCode *callcode;

    if(LOG) printf("log: printf statement begin\n");
    /* new function call code */
    res = newTmpItem(function, voids, 0);
    res->name = getName();
    emit(mid_printf, 0, res, 0, 0);
    callcode = mCode[mcptr - 1];
    mcptr--;
    /* next symbol is lparant */
    nextsy();
    if(sy == lparent){
        /* next symbol is strcon or expression */
        nextsy();
        if(sy == strcon){
            str = getStrcon();
            stab[stab_p++] = str;
            res = newTmpItem(constant, strings, 0);
            res->ref = stab_p - 1;
            //para[count++] = res;
            callcode->paraList[count++] = res;
            if(DEBUG) printf("%s: %s\n", res->name, stab[res->ref]);
            nextsy();
            if(sy == comma){
                nextsy();
                if(s_has(&factbegsys, sy)){
                    res = expressionParse(fsys);
                    //para[count++] = res;
                    callcode->paraList[count++] = res;
                }
            }
        }else if(s_has(&factbegsys, sy)){
            res = expressionParse(fsys);
            //para[count++] = res;
            callcode->paraList[count++] = res;
        }else{
            error(ep_expr);
        }
        if(sy == rparent){
            nextsy();
        }else{
            error(ep_rparent);
        }
    }else{
        error(ep_lparent);
    }
    callcode->count = count;
    mCode[mcptr++] = callcode;
    if(LOG) printf("log: printf statement end\n");
}

void statementParse(struct Set *fsys){
    /* <ident>|<ifstatement>|<whilestatement>|<switchstatement>|<returnstatement>|
        <printfstatement>|<scanfstatement>|<statementsquence> */
    /*
        @sy = statbegsys
        @check semicolon or rbrace
    */
    struct Set *tmpfsys;
    tmpfsys = s_newSet(&statbegsys);
    s_insert(tmpfsys, rbrace);

    if(s_has(fsys, sy)){
        switch(sy){
            case ident:{
                identInfer(fsys);
                /*
                if(sy == semicolon){
                    nextsy();
                }else{
                    error(ep_cs);
                }
                */
                //testsemicolon(tmpfsys, ep_cs);
                break;
            }
            case ifsy : {
                ifstatementParse(fsys);
                break;
            }
            case whilesy : {
                whilestatemnetParse(fsys);
                break;
            }
            case switchsy: {
                switchstatementParse(fsys);
                break;
            }
            case returnsy:{
                returnstatementParse(fsys);
                /*
                if(sy == semicolon){
                    nextsy();
                }else{
                    error(ep_cs);
                }*/
                testsemicolon(tmpfsys, ep_cs);
                break;
            }
            case printfsy: {
                printfstatementParse(fsys);
                /*if(sy == semicolon){
                    nextsy();
                }else{
                    error(ep_cs);
                }*/
                testsemicolon(tmpfsys, ep_cs);
                break;
            }
            case scanfsy: {
                scanfstatementParse(fsys);
                /*
                if(sy == semicolon){
                    nextsy();
                }else{
                    error(ep_cs);
                }
                */
                testsemicolon(tmpfsys, ep_cs);
                break;
            }
            case lbrace: {
                nextsy();
                statementSequenceParse(fsys);
                if(sy == rbrace){
                    nextsy();
                }else{
                    error(ep_lbrace);
                }
                //testNSy(rbrace, fsys, ep_rbrace);
                break;
            }
            case semicolon:{
                nextsy();
                break;
            }
            default: {
                if(sy == casesy){
                    error(case_errplace);
                    nextsy();
                    test(fsys, fsys, ep_state);
                }
                if(sy == defaultsy){
                    nextsy();
                    error(default_errplace);
                    test(fsys, fsys, ep_state);
                }
                if(sy == elsesy){
                    error(else_errplace);
                    nextsy();
                    test(fsys, fsys, ep_state);
                }
            }
        }
    }
    free(tmpfsys);
}

void statementSequenceParse(struct Set *fsys){
    /* {<statement>} */
    /*
        @sy = statbegsys
    */
    struct Set *stafsys;
    stafsys = s_newSet(&statbegsys);
    
    while(s_has(stafsys, sy)){
        statementParse(fsys);
        //test(&statbegsys, &statbegsys, ep_ident);
    }

    free(stafsys);
}

void parametersParse(struct Set *fsys){
    /* <type><ident>{,<type><ident>} */
    /*
        @sy = lparent
    */
    int ty;
    char *name;
    int count = 0;
    struct Set *paranext;

    paranext = s_newSet(0);
    s_insert(paranext, intsy);
    s_insert(paranext, charsy);
    s_insert(fsys, rparent);

    if(LOG) printf("log: parameters declaration begin\n");
    /* next symbol is type */
    nextsy();
    /* next = {int, char}, stop = {rparant} + fsys;*/
    test(paranext, fsys, ep_type);

    while(sy == intsy || sy == charsy){
        ty = sy == intsy ? ints : chars;
        /* next symbol is ident */
        nextsy();
        if(sy == ident){
            nextsy();
        }else{
            error(ep_ident);
            nextsy();
            continue;
        }
        name = getName();
        insertSymbol(name, parameter, ty, 0, 0, 1);
        count++;
        /* next symbol is comma or rparent*/
        if(sy == rparent) break;
        if(sy == comma) nextsy();
        else{
            error(ep_cs);
        }
        test(paranext, fsys, ep_type);
    }
    if(sy != rparent){
        error(ep_rparent);
    }
    tab[curfunc]->paras = count;
    s_erase(fsys, rparent);
    free(paranext);
    if(LOG) printf("log: parameters declaration end\n");
}

void constdel(struct Set *fsys){
    /* const<type><ident>=(<charcon>|<intcon>){,<ident>=(<charcon>|<intcon>)};*/
    /*
        @sy = const
        @check semicolon
    */
    int ty;
    char *name;
    int sign = 0;
    struct Set *s1;

    if(LOG) printf("log: constant declaration begin\n");
    /* test the follow symbol which is intsy or charsy */
    nextsy();
    s1 = s_newSet(0);
    s_insert(s1, charsy);
    s_insert(s1, intsy);
    test(s1, fsys, ep_type);
    free(s1);

    if(sy == charsy || sy == intsy){
        ty = sy == charsy ? chars : ints;
        /* next symbol is ident */
        nextsy();
        /* {,ident = constant} */
        if(sy != ident){
            error(ep_ident);
            return;
        }
        while(sy == ident){
            name = getName();
            /* next symbol is becomes */
            nextsy();
            if(sy != becomes){
                error(ep_becomes);
            }
            /* next symbol is [+-] intcon or intcon/charcon */
            nextsy();
            if(sy == plus || sy == minus){
                sign = (sy == plus) ? 1:-1;
                nextsy();
                if(sy != intcon){
                    error(ep_intcon);
                    inum = 0;
                }
                insertSymbol(name, constant, ty, sign*inum, 0, 0);
                nextsy();
            }else if(sy == charcon || sy == intcon){
                insertSymbol(name, constant, ty, inum, 0, 0);
                nextsy();
            }else{
                insertSymbol(name, constant, ty, 0, 0, 0);
                if(ty == chars) error(ep_charcon);
                else error(ep_intcon);
            }
            /* next symbol is , or ; */
            if(sy == comma){
                nextsy();
            }
        }
        testsemicolon(fsys, ep_type);
    }
    if(LOG) printf("log: constant declaration end\n");
}

void vardel(struct Set *fsys, int ty, char *name){
    /* <type><ident>{,<ident>} */
    /*
        @sy = semicolon | comma | lbrack
        @ty = type, name = <ident>.name
        @check semicolon
    */
    int size;
    struct Set *varnext;
    varnext = s_newSet(0);

    s_insert(varnext, semicolon);
    s_insert(varnext, ident);

    if(LOG) printf("log: variable declaration begin\n");
    /* if sy == ;, we just insert and return */
    if(sy == semicolon){
        insertSymbol(name, variable, ty, 0, 0, 1);
        testsemicolon(fsys, ep_type);
        if(LOG) printf("log: variable declaration end\n");
        return;
    }
    /* if sy == [, we need to deal arrays */
    else if( sy == lbrack){
        /* the next symbol must be a intcon */
        nextsy();
        size = inum;
        if(sy == intcon){
            nextsy();
        }else{
            error(arr_int);
            size = 0;
            if(sy == ident){
                nextsy();
            }
        }
        /* the next symbol must be a ] */
        insertSymbol(name, array, ty, 0, 0, size);
        if(sy == rbrack){
            nextsy();
        }else{
            error(arr_rbract);
        }
        /* the next symbol is comma or ; */
        if(sy == comma) nextsy();    /* sy will be ident */
    }
    /* sy == comma */
    else if(sy == comma){
        insertSymbol(name, variable, ty, 0, 0, 1);
        /* read next symbo which is ident or ; */
        nextsy();
    }
    if(s_has(fsys, sy)){
        test(varnext, fsys, ep_cs);
    }else{
        test(varnext, fsys, ep_ident);
    }
    /* parse next variable */
    if(sy == semicolon){
        testsemicolon(fsys, ep_type);
        if(LOG) printf("log: variable declaration end\n");
        return;
    }else if(sy == ident){
        /* next symbo must be ident */
        name = getName();
        nextsy();
        /* next symbol must be , or ; or [ */
        if(sy == comma || sy == semicolon || lbrack){
            vardel(fsys, ty, name);
        }else{
            error(ep_cs);
            test(fsys, fsys, ep_type);
        }
    }
    free(varnext);
    if(LOG) printf("log: variable declaration end\n");
    return;
}

void funcdel(int ty, char *name, int isMain){
    struct Set *funcsys;
    struct Set *stafsys;
    struct Set *fsys;
    struct Set *ftmp;
    struct Set *varnext;

    funcsys = s_newSet(&funcbegsys);
    stafsys = s_newSet(&statbegsys);
    fsys = s_newSet(&funcbegsys);
    s_insert(fsys, rbrace);
    ftmp = s_newSet(0);
    varnext = s_newSet(fsys);
    s_plus(fsys, stafsys);
    s_insert(varnext, ident);

    if(LOG) printf("log: function declaration begin\n");
    /* insert function and alloc new block */
    curfunc = insertSymbol(name, function, ty, 0, mcptr, 0);
    tab[curfunc]->isMain = isMain;
    emit(mid_label, 0, 0, 0, tab[curfunc]);
    insertBlock();

    /* need to check parameters*/
    if(sy == lparent){
        if(!isMain){
            parametersParse(fsys);
            nextsy();
        }else{
            nextsy();
            if(sy == rparent) nextsy();
            else error(ep_rparent);
        }     
    }
    if(tab[curfunc]->paras <= 4) btab[btab_p - 1]->dx = 4;
    btab[btab_p - 1]->pdx = btab[btab_p - 1]->dx;
    btab[btab_p - 1]->dx = 0;
    btab[btab_p - 1]->dx += SAVESTACK;
    /* next is lbrace */
    s_insert(ftmp, lbrace);
    test(ftmp, fsys, ep_lbrace);
    s_erase(ftmp, lbrace);
    /* next is funcbegsys + statebegsys*/
    nextsy();
    //test(fsys, fsys, ep_type);
    /* declaration */
    /* constant */
    while(sy == constsy){
        constdel(fsys);
        test(fsys, fsys, ep_type);
    }
    /* variable */
    s_erase(funcsys, constsy);
    while(sy == intsy || sy == charsy){
        ty = sy == intsy ? ints : chars;
        /* read next symbol, it must be ident*/
        nextsy();
        if(sy == ident){
            nextsy();
        }else{
            error(ep_ident);
            //nextsy();
            test(varnext, fsys, ep_ident);
            if(sy != ident) continue;
        }
        name = getName();
        /* read next symbol, it could be a comma, semicolon, lbrace or function*/
        if(sy == comma || sy == semicolon || sy == lbrack){
            vardel(fsys, ty, name);
        }else{
            error(ep_cs);
            //nextsy();
            test(fsys, fsys, ep_ident);
            continue;
        }
        test(fsys, fsys, ep_ident);
    }

    s_insert(stafsys, semicolon);
    s_plus(ftmp, stafsys);
    s_insert(ftmp, rbrace);

    if(s_has(stafsys, sy)){
        statementSequenceParse(ftmp);
    }
    if(sy == rbrace){
        nextsy();
    }else{
        error(ep_rbrace);
    }
    tab[curfunc]->end = mcptr;
    free(fsys);
    free(ftmp);
    free(stafsys);
    free(varnext);
    if(tab[curfunc]->type != voids && !tab[curfunc]->hasRet){
        error(func_ret);
    }
    curlev--;
    if(LOG) printf("log: function declaration end\n");
}

void programParse(){
    insertBlock();
    //btab[0]->dx = 0;
    nextsy();
    int bdx = 0;
    /* declaration */
    int kd;
    int ty;
    char *name;

    struct Set *progfsys;
    struct Set *varn;

    //s_clone(&progfsys, &progbegsys);
    progfsys = s_newSet(&progbegsys);
    varn = s_newSet(progfsys);
    s_insert(varn, ident);

    test(progfsys, progfsys, ep_prog);

    if(LOG) printf("log: program begin\n");

    /* constant declaration */
    while(sy == constsy){
        constdel(progfsys);
        test(progfsys, progfsys, ep_prog);
    }
    s_erase(progfsys, constsy);
    /* variable declaration */
    while(sy == intsy || sy == charsy){
        ty = sy == intsy ? ints : chars;
        /* read next symbol, it must be ident*/
        nextsy();
        if(sy != ident){
            error(ep_ident);
            nextsy();
            test(varn, progfsys, ep_prog);
            if(sy != ident) continue;
        }
        name = getName();
        /* read next symbol, it could be a comma, semicolon, lbrace or function*/
        nextsy();
        /* This is function */
        if(sy == lparent || sy == lbrace){
            break;
        }
        if(sy == comma || sy == semicolon || sy == lbrack){
            vardel(progfsys, ty, name);
        }else{
            error(ep_cs);
            nextsy();
            test(progfsys, progfsys, ep_prog);
            continue;
        }
        test(progfsys, progfsys, ep_ident);
    }

    /* function declaration */
    /* here are some case */
    /* case 1: int abc {}, sy == { or sy == (, and we remembered int and ident*/
    /* case 2 : void abc, sy == void */
    if( sy == lparent || sy == lbrace){
        funcdel(ty, name, 0);
        test(progfsys, progfsys, ep_prog);
    }

    while(sy == voidsy || sy == intsy || sy == charsy){
        ty = sy == intsy ? ints : (sy == charsy ? chars : voids);
        /* read next symbol which must be ident */
        nextsy();
        name = getName();
        strcpy(name, id);
        if(sy != ident && sy != mainsy){
            error(ep_ident);
            test(progfsys, progfsys, ep_prog);
            continue;
        }
        /* read next symbol which must be ( or {, need funcdel to check */
        if(sy == mainsy){
            break;
        }
        if(sy == ident){
            nextsy();
            funcdel(ty, name, 0);
        }
        test(progfsys, progfsys, ep_prog);
    }
    /* main function parse */
    if(sy == mainsy){
        nextsy();
        funcdel(ty, name, 1);
    }else{
        error(ep_main);
        //printf("error: undefined reference to 'main'\n");
    }
    if(LOG) printf("log: program end\n");
}
