#ifndef _TABLE_H
#define _TABLE_H

#define TABMLX 1000
#define BTABMLX 100
#define STABMLX 1000
#define ATABLMX 1000
#define SAVESTACK 10

struct SymItem{
    char *name;         /* id */
    int kind;           /* kind */
    int type;           /* type */
    int lev;            /* the block level */
    int adr;            /* a constant or a index in tab or in stab */
    int link;           /* the last item in the same level */
    int ref;            /* function code start or stab index */
    int end;            /* function code end */
    int size;           /* arrays size */
    int paras;          /* the number of parameters */
    int isTmp;          /* is temp variable */
    int num;            /* number */
    int hasRet;         /* sign of function return */
    int count;          /* the num of using*/
    int reg;            /* register allcation */
    int loop;           /* sign while label */
    int dirty;          /* when dirty, need to save */
    int isModified;     /* node modified in dag */
    int creg;           /* count assign reg */
    int isMain;         /* is main function */
};

struct SymBlock{
    int cur;            /* the function index in tab */
    int last;           /* the last position in tab */
    int dx;             /* the dx size */
    int tdx;            /* the tmp variable size */
    int pdx;            /* the arguments size */
};



extern struct SymItem *tab[TABMLX];
extern struct SymBlock *btab[BTABMLX];
extern char *stab[STABMLX];

extern int curlev;
extern int tab_p;
extern int btab_p;
extern int stab_p;
extern int varId;

int locSymbol(char *id);
void insertBlock();
int insertSymbol();


#endif