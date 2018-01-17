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
#include "mipsGen.h"
#include "optimizer.h"

int graph[NBLOCKS][NBLOCKS] = {0};
struct BasicBlock *BBs[NBLOCKS] = {0};
int nBBs = 0;

int RIG_graph[RIGSIZE][RIGSIZE] = {0};
int RIG_edges[RIGSIZE] = {0};
int RIG_visited[RIGSIZE] = {0};
int RIG_hash2vec[HASHTABLESIZE] = {0};
int RIG_vec2hash[RIGSIZE] = {0};
int RIG_n = 0;

struct SymItem *countTab[HASHTABLESIZE] = {0};
int count_p = 0;

struct SymItem *hashTab[HASHTABLESIZE] = {0};
struct Set use[NBLOCKS];
struct Set def[NBLOCKS];
struct Set l_in[NBLOCKS];
struct Set l_out[NBLOCKS];

int colorSquence[100];
int spillvar[100];
int colorp = 0;
int spillp = 0;

/* CSE */
struct FCode *DAG_vex[DAGSEIZE] = {0};              /* DAG 四元式图节点数组 */
int DAG_vex_n = 0;                                  /* DAG 四元式图数组下标 */
struct SymItem *DAG_nodeStack[DAGSEIZE] = {0};      /* DAG 四元式操作数栈 */
int DAG_NS_p = 0;                                   /* DAG 四元式操作数栈下标 */
int DAG_nodeIndex[DAGSEIZE] = {0};                  /* DAG 四元式操作数栈映射 */

void func2BBs(struct SymItem *func);

void insert2CountTab(struct SymItem *vct){
    int i;
    
    if(vct == 0) return;
    if(vct->lev == 0 || vct->kind == parameter || vct->kind == constant) return;

    for(i = 0; i < count_p; i++){
        if(countTab[i] == vct){
            break;
        }
    }

    if(i == count_p){
        countTab[count_p++] = vct;
    }
}

void assignGlobalRegWithCount(){
    int i, j;
    struct SymItem *tmp;
    /* sort */
    for(i = 0; i < count_p - 1; i++){
        for(j = i + 1; j < count_p; j++){
            if(countTab[i]->count < countTab[j]->count){
                tmp = countTab[i];
                countTab[i] = countTab[j];
                countTab[j] = tmp;
            }
        }
    }
    /* assign */
    for(i = 0; i < GLOBALREQ && i < count_p; i++){
        countTab[i]->creg = reg_s0 + i;
        if(DEBUG) printf("count : %s -> s%d\n", countTab[i]->name, countTab[i]->creg - reg_s0);
    }
}

void printGraph(){
    int i, j;
    for(i = 0; i < nBBs; i++){
        for(j = 0; j < nBBs; j++){
            if(graph[i][j]) printf("Block_%d --> Block_%d\n", i, j);
        }
    }
}

void printSet(){
    int i, j;

    for(i = 1; i < nBBs - 1; i++){
        printf("def[%d]: {", i);
        for(j = 0; j < SETSIZE; j++){
            if(s_has(&def[i], j)) printf("%s, ", hashTab[j]->name);
        }
        printf("} ");
        printf("use[%d]: {", i);
        for(j = 0; j < SETSIZE; j++){
            if(s_has(&use[i], j)) printf("%s, ", hashTab[j]->name);
        }
        printf("} ");
        printf("in[%d]: {", i);
        for(j = 0; j < SETSIZE; j++){
            if(s_has(&l_in[i], j)) printf("%s, ", hashTab[j]->name);
        }
        printf("} ");
        printf("out[%d]: {", i);
        for(j = 0; j < SETSIZE; j++){
            if(s_has(&l_out[i], j)) printf("%s, ", hashTab[j]->name);
        }
        printf("}\n");
    }
}

void printAnySet(struct Set *s){
    int j;
    printf("{");
    for(j = 0; j < SETSIZE; j++){
        if(s_has(s, j)) printf("%s, ", hashTab[j]->name);
    }
    printf("}\n");
}

void allocBB(int head, int tail){
    struct BasicBlock *bb;
    bb = (struct BasicBlock *)malloc(sizeof(struct BasicBlock));
    if(bb == 0){
        fatal(3);
    }
    bb->num = nBBs;
    bb->head = head;
    bb->tail = tail;
    BBs[nBBs++] = bb;
    if(DEBUG) printf("[insert block_%d: head: %d, tail = %d]\n", bb->num, bb->head, bb->tail);
}

void func2BBs(struct SymItem *func){
    
    int i = 0;;
    struct FCode *c;
    int head, tail;
    nBBs = 0;

    /* scan midcodes of function and mark */
    c = mCode[func->ref];
    c->bbsign = 1;
    for(i = func->ref + 1; i < func->end; i++){
        c = mCode[i];
        if(c->type == mid_goto){
            mCode[c->res->adr]->bbsign = 1;
            mCode[i + 1]->bbsign = 1;
        }
        if(c->type == mid_goFalse || c->type == mid_goTrue){
            mCode[c->res->adr]->bbsign = 1;
            mCode[i + 1]->bbsign = 1;
        }
    }
    /* scan and create bblock */
    allocBB(-1, -1);
    head = func->ref;
    tail = func->end - 1;
    c = mCode[func->ref];
    c->belong2BB = nBBs;
    for(i = func->ref + 1; i < func->end; i++){
        c = mCode[i];
        if(c->bbsign){
            tail = i;
            allocBB(head, tail);
            head = tail;
        }
        c->belong2BB = nBBs;
    }
    if(head <= tail) allocBB(head, i);
    allocBB(-1, -1);
    if(DEBUG){
        for(i = func->ref; i < func->end; i++){
            c = mCode[i];
            printf("code_%d: belong2BB: %d, bbsign: %d\n", i, c->belong2BB, c->bbsign);
        }
    }
}

void genCFG(struct SymItem *func){
    int i, j;
    struct FCode *c;
    struct FCode *tar;
    struct BasicBlock *bb;

    /* init graph */
    for(i = 0; i < nBBs; i++){
        for(j = 0; j < nBBs; j++){
            graph[i][j] = 0;
        }
    }
    /* scan basicblcoks */
    graph[0][1] = 1;
    for(i = 1; i < nBBs - 1; i++){
        bb = BBs[i];
        c = mCode[bb->tail - 1];
        if(c->type == mid_goto){
            tar = mCode[c->res->adr];
            graph[i][tar->belong2BB] = 1;
        }else if(c ->type == mid_goFalse || c->type == mid_goTrue){
            tar = mCode[c->res->adr];
            graph[i][tar->belong2BB] = 1;
            graph[i][i+1] = 1;
        }else{
            graph[i][i+1] = 1;
        }
    }
    if(DEBUG) printGraph();
}

int selectNode(){
    int i, j;
    int maxindex = -1;
    int max = -1;
    int runCount;
    /* first find a node with edges < GLOBALREQ */
    for(i = 0; i < RIG_n; i++){
        if(RIG_edges[i] < GLOBALREQ && !RIG_visited[i]){
            maxindex = i;
            max = RIG_edges[i];
        }
    }
    /* if we do not find a node with smaller edges, try to find a node with max edges*/
    if(maxindex == -1){
        for(i = 0; i < RIG_n; i++){
            if(RIG_edges[i] > max && !RIG_visited[i]){
                maxindex = i;
                max = RIG_edges[i];
            }
        }
    }
    if(maxindex != -1){
        RIG_visited[maxindex] = 1;
        for(i = 0; i < RIG_n; i++){
            if(RIG_graph[maxindex][i]){
                RIG_edges[i] -= 1;
            }
        }
    }
    return maxindex;
}

void in2inSet(struct Set *s, struct SymItem *vct, int bbn){
    if(vct == 0) return;
    if(s_has(&use[bbn], vct->num) || s_has(&def[bbn], vct->num)){
        s_insert(s, vct->num);
    }
}

void out4inSet(struct Set *s, struct SymItem *vct, int bbn){
    if(vct == 0) return;
    if(s_has(&use[bbn], vct->num) || s_has(&def[bbn], vct->num)){
        s_erase(s, vct->num);
    }
}

void insertRIGEdge(struct SymItem *vct, struct Set *s, int bbn){
    int i, j;
    int v1, v2;
    j = vct->num;
    if(!s_has(&use[bbn], vct->num) && !s_has(&def[bbn], vct->num)){
        return;
    }
    if(DEBUG){
        printf("%s ", vct->name);
        printAnySet(s);
    }
    if(RIG_hash2vec[j] == -1){
        // map vec 1 hash
        RIG_vec2hash[RIG_n] = j;
        RIG_hash2vec[j] = RIG_n++;
    }
    for(i = 0; i < SETSIZE; i++){
        if(s_has(s, i)){
            if(RIG_hash2vec[i] == -1){
                // map vec 2 hash
                RIG_vec2hash[RIG_n] = i;
                RIG_hash2vec[i] = RIG_n++;
            }
            v1 = RIG_hash2vec[j];
            v2 = RIG_hash2vec[i];
            if(v1 != v2 && (!RIG_graph[v1][v2] || !RIG_graph[v1][v2])){
                RIG_graph[v1][v2] = 1;
                RIG_graph[v2][v1] = 1;
                RIG_edges[v1]++;
                RIG_edges[v2]++;
            }
        }
    }
}

void genRIG(){
    int i, j, k;
    int v1, v2;
    int buf[1000];
    int n = 0;
    int snode;
    int vec = 0;
    struct Set *neighbor;
    struct Set *outSet;
    struct Set *inSet;
    struct Set *extraSet;
    int h, t;
    struct FCode *c;
    int type;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int bbn;
    /* init */
    neighbor = s_newSet(0);
    for(i = 0; i < RIGSIZE; i++){
        for(j = 0; j < RIGSIZE; j++){
            RIG_graph[i][j] = 0;
        }
        RIG_edges[i] = 0;
        RIG_visited[i] = 0;
        RIG_hash2vec[i] = -1;
    }
    for(i = 0; i < HASHTABLESIZE; i++){
        RIG_hash2vec[i] = -1;
    }
    RIG_n = 0;                  /* this is size of rig_graph*/
    /* create rig graph */
    /*
    for(i = 1; i < nBBs - 1; i++){
        n = 0;
        // scan in[i] and store var in buf
        for(j = 0; j < 1000; j++){
            if(s_has(&l_in[i], j)){
                buf[n++] = j;
                if(RIG_hash2vec[j] == -1){
                    // map vec 2 hash
                    RIG_vec2hash[RIG_n] = j;
                    RIG_hash2vec[j] = RIG_n++;
                }
            }
        }
        // let var interference in RIG_graph 
        for(j = 0; j < n - 1; j++){
            for(k = j + 1; k < n; k++){
                v1 = buf[j];
                v1 = RIG_hash2vec[v1];
                v2 = buf[k];
                v2 = RIG_hash2vec[v2];
                if(!RIG_graph[v1][v2]){
                    // no direction
                    RIG_graph[v1][v2] = 1;
                    RIG_graph[v2][v1] = 1;
                    RIG_edges[v1]++;
                    RIG_edges[v2]++;
                }
            }
        }
    }
    */
    for(i = 1; i < nBBs - 1; i++){
        inSet = s_newSet(&l_out[i]);
        extraSet = s_newSet(0);
        h = BBs[i]->head;
        t = BBs[i]->tail;
        for(j = t - 1; j >= h; j--){
            outSet = s_newSet(inSet);
            c = mCode[j];
            type = c->type;
            a = c->a;
            b = c->b;
            res = c->res;
            bbn = c->belong2BB;
            if(type == mid_cal){
                in2inSet(inSet, a, i);
                in2inSet(inSet, b, i);
                out4inSet(inSet, res, i);
                insertRIGEdge(res, outSet, i);
                //s_insert(extraSet, a->num);
                //s_insert(extraSet, b->num);
                //s_insert(extraSet, res->num);
            }else if(type == mid_stAry){
                in2inSet(inSet, a, i);
                in2inSet(inSet, b, i);
                //s_insert(extraSet, a->num);
                //s_insert(extraSet, b->num);
            }else if(type == mid_ldAry){
                in2inSet(inSet, b, i);
                out4inSet(inSet, res, i);
                insertRIGEdge(res, outSet, i);
                //s_insert(extraSet, b->num);
                //s_insert(extraSet, res->num);
            }else if(type == mid_goFalse || type == mid_goTrue ||type == mid_goto){
                in2inSet(inSet, a, i);
                in2inSet(inSet, b, i);
                //s_insert(extraSet, a->num);
                //s_insert(extraSet, b->num);
            }else if(type == mid_cpy){
                in2inSet(inSet, a, i);
                out4inSet(inSet, res, i);
                //s_insert(outSet, a->num);
                insertRIGEdge(res, outSet, i);
                //s_erase(outSet, a->num);
                //s_insert(extraSet, a->num);
                //s_insert(extraSet, res->num);
            }else if(type == mid_call || type == mid_printf || type == mid_scanf){
                for(k = 0; k < c->count; k++){
                    in2inSet(inSet, c->paraList[k], i);
                    //s_insert(extraSet,  c->paraList[k]->num);
                }
                if(res != 0){
                    out4inSet(inSet, res, i);
                    insertRIGEdge(res, outSet, i);
                    //s_insert(extraSet, res->num);
                }
            }else if(type == mid_ret){
                in2inSet(inSet, res, i);
                //s_insert(extraSet, res->num);
            }
            free(outSet);
        }
        /* extra */
        for(k = 0; k < SETSIZE; k++){
            if(s_has(&use[i], k) || s_has(&def[i], k)){
                insertRIGEdge(hashTab[k], &use[i], i);
                insertRIGEdge(hashTab[k], &def[i], i);
            }
        }
        free(inSet);
        free(extraSet);
    }
    /* generate rig sequence */
    colorp = 0;
    spillp = 0;
    snode = selectNode();
    while(snode != -1){
        if(RIG_edges[snode] < GLOBALREQ) colorSquence[colorp++] = snode;
        else spillvar[spillp++] = snode;
        snode = selectNode();
    }
    /* coloring */
    for(i = colorp - 1; i >= 0; i--){
        s_init(neighbor);
        /* scan node which connect to vec*/
        vec = colorSquence[i];
        for(j = 0; j < RIG_n; j++){
            if(RIG_graph[vec][j]){
                k = RIG_vec2hash[j];
                if(hashTab[k]->reg != -1){
                    s_insert(neighbor, hashTab[k]->reg);
                }
            }
        }
        /* assign a reg to vec */
        for(j = 0 + reg_s0; j < GLOBALREQ + reg_s0; j++){
            if(!s_has(neighbor, j)){
                k = RIG_vec2hash[vec];
                hashTab[k]->reg = j;
                break;
            }
        }
        k = RIG_vec2hash[vec];
        if(DEBUG) printf("%s -> s%d\n", hashTab[k]->name, hashTab[k]->reg - reg_s0);
    }
    free(neighbor);
}

void useAnaly(struct SymItem *var, int bbn){
    if(var == 0) return;
    insert2CountTab(var);
    if(var->kind == constant || var->kind == parameter || var->lev == 0 || var->isTmp) var->count++;
    else if(!s_has(&def[bbn], var->num)){
        s_insert(&use[bbn], var->num);
        hashTab[var->num] = var;
    }
}

void defAnaly(struct SymItem *var, int bbn){
    if(var == 0) return;
    insert2CountTab(var);
    if(var->kind == constant || var->kind == parameter || var->lev == 0 || var->isTmp) var->count++;
    else if(!s_has(&use[bbn], var->num)){
        s_insert(&def[bbn], var->num);
        hashTab[var->num] = var;
    }
}

void genLiveIntervals(struct SymItem *func){
    int i, j;
    struct BasicBlock *bb;
    int bbn;
    struct Set *stmp;
    int stop;
    int type;
    struct FCode *c;
    struct FCode *tar;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;

    /* init */
    count_p = 0;
    func2BBs(func);
    genCFG(func);
    /* scan mid codes and generate use and def */
    /* and count refrence */
    for(i = 0; i < nBBs; i++){
        s_init(&use[i]);
        s_init(&def[i]);
        s_init(&l_in[i]);
        s_init(&l_out[i]);
    }
    for(i = func->ref; i < func->end; i++){
        c = mCode[i];
        type = c->type;
        a = c->a;
        b = c->b;
        res = c->res;
        bbn = c->belong2BB;
        if(type == mid_cal){
            useAnaly(a, bbn);
            useAnaly(b, bbn);
            defAnaly(res, bbn);
        }else if(type == mid_stAry){
            useAnaly(a, bbn);
            useAnaly(b, bbn);
            //useAnaly(res, bbn);
        }else if(type == mid_ldAry){
            //useAnaly(a, bbn);
            useAnaly(b, bbn);
            defAnaly(res, bbn);
        }else if(type == mid_goFalse || type == mid_goTrue ||type == mid_goto){
            useAnaly(a, bbn);
            useAnaly(b, bbn);
            if(i > c->res->adr){
                for(j=c->res->adr; j <= i; j++){
                    if(mCode[j]->a != 0) mCode[j]->a->count += LOOPWEIGHT;
                    if(mCode[j]->b != 0) mCode[j]->b->count += LOOPWEIGHT; 
                    if(mCode[j]->res != 0) mCode[j]->res->count += LOOPWEIGHT; 
                }
            }
        }else if(type == mid_cpy){
            useAnaly(a, bbn);
            defAnaly(res, bbn);
        }else if(type == mid_call || type == mid_printf || type == mid_scanf){
            for(j = 0; j < c->count; j++){
                useAnaly(c->paraList[j], bbn);
            }
            if(res != 0) defAnaly(res, bbn);
        }else if(type == mid_ret){
            useAnaly(res, bbn);
        }
    }
    /* check in and out */
    stop = 0;
    while(!stop){
        stop = 1;
        for(i = nBBs - 2; i >= 0; i--){
            /* out[B] = U(in[s]) */
            stmp = s_newSet(0);
            for(j = 0; j < nBBs; j++) {
                if(graph[i][j]) s_plus(stmp, &l_in[j]);
            }
            s_init(&l_out[i]);
            s_plus(&l_out[i], stmp);
            /* in[B] = use[B]U(out[B] - def[B]) */
            s_init(stmp);
            s_plus(stmp, &l_out[i]);
            s_minus(stmp, &def[i]);
            s_plus(stmp, &use[i]);
            if(!s_equal(&l_in[i], stmp)){
                s_init(&l_in[i]);
                s_plus(&l_in[i], stmp);
                stop = 0;
            }
            free(stmp);
        }
    }
    /* debug */
    if(DEBUG) printSet();
    genRIG();
    assignGlobalRegWithCount();
}

void DAG2Code(){
    int i;
    for(i = 0; i < DAG_vex_n; i++){
        mOptCode[mOptcptr++] = DAG_vex[i];
        DAG_vex[i] = 0;
    }
    DAG_vex_n = 0;
    DAG_NS_p = 0;
}

void insert2DAG(struct FCode *c){
    int i;
    int type;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int a_ind;
    int b_ind;
    int res_ind;
    struct FCode *vex;
    int v_ind;
    int op;
    int rev;
    int revStart;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    type = c->type;

    /* find a index and b and res index in node stack */
    a_ind = -1;
    b_ind = -1;
    res_ind = -1;
    for(i = 0; i < DAG_NS_p; i++){
        if(DAG_nodeStack[i] == a){
            a_ind = i;
        }
        if(DAG_nodeStack[i] == b){
            b_ind = i;
        }
        if(DAG_nodeStack[i] == res){
            res_ind = i;
        }
    }
    /* handel */
    if(type == mid_cpy){
        rev = 0;
        /* if we find a just use a_ind if not then push a into stack*/
        if(a_ind < 0){
            DAG_nodeStack[DAG_NS_p] = a;
            DAG_nodeIndex[DAG_NS_p] = DAG_NS_p;
            a_ind = DAG_NS_p;
            DAG_NS_p++;
        }else{
            for(i = 0; i < DAG_vex_n; i++){
                if(DAG_vex[i]->res == a && DAG_vex[i]->res->isTmp){
                    rev = 1;
                    revStart = i;
                    vex = DAG_vex[i];
                }
            }
        }
        /* if res not in stack need to push res into stack */
        if(res_ind < 0){
            DAG_nodeStack[DAG_NS_p] = res;
            DAG_nodeIndex[DAG_NS_p] = DAG_NS_p;
            res_ind = DAG_NS_p;
            DAG_NS_p++;
        }else{
            //DAG_nodeStack[res_ind] = 0;
            //DAG_nodeIndex[res_ind] = -1;
            //DAG2Code();
            for(i = 0; i < DAG_vex_n; i++){
                if(DAG_vex[i]->a == res || DAG_vex[i]->b == res){
                    DAG_vex[i]->isOld = 1;
                }
            }
        }
        /* then gen dag vex maybe need to remap a*/
        if(VARIABLEFOLDING && rev){
            DAG_nodeIndex[a_ind] = res_ind;
            for(i = revStart + 1 ; i < DAG_vex_n; i++){
                if(DAG_vex[i]->a == res){
                    DAG_vex[i]->a = DAG_nodeStack[DAG_nodeIndex[a_ind]];
                }
                if(DAG_vex[i]->b == res){
                    DAG_vex[i]->b = DAG_nodeStack[DAG_nodeIndex[a_ind]];
                }
            }
            vex->res = DAG_nodeStack[DAG_nodeIndex[a_ind]];
        }else{
            c->a = DAG_nodeStack[DAG_nodeIndex[a_ind]];
            DAG_vex[DAG_vex_n] = c;
            DAG_vex_n++;
        }
    }else if(type == mid_cal){
        /* if we find a just use a_ind if not then push a into stack*/
        if(a_ind < 0){
            DAG_nodeStack[DAG_NS_p] = a;
            DAG_nodeIndex[DAG_NS_p] = DAG_NS_p;
            a_ind = DAG_NS_p;
            DAG_NS_p++;
        }
        /* if we find b just use b_ind if not then push a into stack*/
        if(b_ind < 0){
            DAG_nodeStack[DAG_NS_p] = b;
            DAG_nodeIndex[DAG_NS_p] = DAG_NS_p;
            b_ind = DAG_NS_p;
            DAG_NS_p++;
        }
        /* to check the res in dag with op */
        vex = 0;
        v_ind = -1;
        for(i = 0; i < DAG_vex_n; i++){
            if(DAG_vex[i]->a == a && DAG_vex[i]->b == b && DAG_vex[i]->op == op && !DAG_vex[i]->isOld){
                vex = DAG_vex[i];
            }
        }
        /* if there is no a same expr need to insert a vex*/
        if(!vex){
            c->a = DAG_nodeStack[DAG_nodeIndex[a_ind]];
            c->b = DAG_nodeStack[DAG_nodeIndex[b_ind]];
            if(CONSTANTFOLDING && c->a->kind == constant && c->b->kind == constant && c->res->isTmp){
                switch(op){
                    case plus: c->res->adr = c->a->adr + c->b->adr;break;
                    case minus: c->res->adr = c->a->adr - c->b->adr;break;
                    case times: c->res->adr = c->a->adr * c->b->adr;break;
                    case idiv: c->res->adr = c->a->adr / c->b->adr;break;
                    default: printf("constant folding error!\n");
                }
                c->res->kind = constant;
                vex = c;
            }else{
                vex = c;
                DAG_vex[DAG_vex_n] = c;
                DAG_vex_n++;
            }
        }
        /* if res not in stack need to push res into stack */
        if(res_ind < 0){
            DAG_nodeStack[DAG_NS_p] = res;
            DAG_nodeIndex[DAG_NS_p] = DAG_NS_p;
            res_ind = DAG_NS_p;
            DAG_NS_p++;
        }else{
            //DAG_nodeStack[res_ind] = 0;
            //DAG_nodeIndex[res_ind] = -1;
            //DAG2Code();
            for(i = 0; i < DAG_vex_n; i++){
                if(DAG_vex[i]->a == res || DAG_vex[i]->b == res){
                    DAG_vex[i]->isOld = 1;
                }
            }
        }
        for(i = 0; i < DAG_NS_p; i++){
            if(DAG_nodeStack[i] == vex->res){
                v_ind = i;
            }
        }
        DAG_nodeIndex[res_ind] = v_ind;
    }
}

struct SymItem *remapItem(struct SymItem *vct){
    int i;
    int ind;

    ind = -1;
    for(i = 0; i < DAG_NS_p; i++){
        if(DAG_nodeStack[i] == vct){
            ind = i;
        }
    }

    if(ind == -1) return vct;
    return DAG_nodeStack[DAG_nodeIndex[ind]];
}

void CSE4block(int bbn){
    struct BasicBlock *bb = BBs[bbn];
    int h = bb->head;
    int t = bb->tail;
    int i;
    int j;
    struct FCode *c;
    int type;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int genFlag = 0;

    for(i = h; i < t; i++){
        c = mCode[i];
        a = c->a;
        b = c->b;
        res = c->res;
        type = c->type;
        if(type == mid_cal || type == mid_cpy){
            insert2DAG(c);
        }else{
            DAG_vex[DAG_vex_n++] = c;
            if(type == mid_call || type == mid_printf || type == mid_scanf){
                for(j = 0; j < c->count; j++){
                   c->paraList[j] = remapItem(c->paraList[j]);
                }
                for(j = 0; j < DAG_vex_n; j++){
                    if(DAG_vex[j]->a != 0 && DAG_vex[j]->b != 0 && (DAG_vex[j]->a->lev == 0 || DAG_vex[j]->b->lev == 0)){
                        DAG_vex[j]->isOld = 1;
                    }
                }
            }
            if(type == mid_goFalse){
                c->a = remapItem(a);
                c->b = remapItem(b);
            }
            if(type == mid_ldAry){
                c->res = remapItem(res);
                c->b = remapItem(b);
            }
            if(type == mid_stAry){
                c->b = remapItem(b);
                c->a = remapItem(a);
            }
            if(type == mid_ret){
                c->res = remapItem(res);
            }
        }
        /*
        if(genFlag){
            DAG2Code();
            genFlag = 0;
        }
        */
    }
    DAG2Code();
}

void genCSE(){
    int i, j;
    int index;
    struct SymBlock *locBlock;
    struct SymItem *func;

    for(index = 1; index < btab_p; index++){
        locBlock = btab[index];
        func = tab[locBlock->cur];
        /* init */
        func2BBs(func);
        func->ref = mOptcptr;
        /* scan for every block */
        for(i = 1; i < nBBs; i++){
            CSE4block(i);
        }
        func->end = mOptcptr;
    }
}