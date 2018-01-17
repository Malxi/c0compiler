#ifndef _MIDCODE_H
#define  _MIDCODE_H

#define CODELMX 2000

enum midCodeKind{
    mid_cal,
    mid_stAry, mid_ldAry,
    mid_goFalse, mid_goTrue, mid_goto,
    mid_cpy,
    mid_label,
    mid_call, mid_ret,
    mid_printf, mid_scanf
};

struct FCode {
    int type;
    int op;
    int count;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    struct SymItem *paraList[20];
    int bbsign;
    int belong2BB;
    int isOld;
};

extern struct FCode *mCode[CODELMX];
extern int mcptr;

extern struct FCode *mOptCode[CODELMX];
extern int mOptcptr;

void emit(int type, int op, struct SymItem *a,  struct SymItem *b, struct SymItem *res);
void outputMcode(void);
void outputOPMcode(void);
void copyOPMcode(void);
#endif