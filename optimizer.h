#ifndef _OPTIMIZER_H
#define _OPTIMIZER_H

#define NBLOCKS 100
#define HASHTABLESIZE 1000
#define LOOPWEIGHT 100
#define RIGSIZE 1000
#define GLOBALREQ 8
#define DAGSEIZE 1000

struct BasicBlock{
    int num;
    int head;
    int tail;
};

extern int graph[NBLOCKS][NBLOCKS];
extern struct BasicBlock *BBs[NBLOCKS];
extern int nBBs;
extern struct Set l_in[NBLOCKS];
extern struct Set l_out[NBLOCKS];
extern struct Set use[NBLOCKS];
extern struct Set def[NBLOCKS];
extern struct SymItem *hashTab[HASHTABLESIZE];
extern int RIG_vec2hash[RIGSIZE];
extern int colorSquence[100];
extern int spillvar[100];
extern int colorp;
extern int spillp;
extern struct SymItem *countTab[HASHTABLESIZE];
extern int count_p;

void genCFG(struct SymItem *func);
void genLiveIntervals(struct SymItem *func);
void genCSE();

#endif