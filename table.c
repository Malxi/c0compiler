#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"
#include "table.h"

struct SymItem *tab[TABMLX];
struct SymBlock *btab[BTABMLX];
char *stab[STABMLX];
struct SymBlock *atab[ATABLMX];
int display[100];

int curlev = -1;
int tab_p = 1;
int btab_p = 0;
int stab_p = 0;
int varId = 0;


int locSymbol(char *name){
    int j;
    int i;

    /* get current block */
    i = btab_p - 1; 
    tab[0]->name = name;
    /* search current block */
    j = btab[i]->last;
    while(strcmp(tab[j]->name, name) != 0){
        j = tab[j]->link;
    }
    /* if j == 0 && i > 0 search 0 block */
    if(i > 0 && j == 0){
        j = btab[0]->last;
        while(strcmp(tab[j]->name, name) != 0){
            j = tab[j]->link;
        }
    }
    if(j == 0) return 0;
    return j;
}

void insertBlock(){
    curlev++;
    /* alloc a new symblock and init */
    btab[btab_p] = (struct SymBlock *)malloc(sizeof(struct SymBlock));
    btab[btab_p]->cur = tab_p - 1;
    btab[btab_p]->last = 0;
    btab[btab_p]->dx = 0;
    btab[btab_p]->tdx = 0;
    btab[btab_p]->pdx = 0;
    btab_p++;
    if(btab_p >= TABMLX) fatal(2);
}

int insertSymbol(char *name, int kind, int type, int adr, int ref, int size){
    int j;
    int l;
    int dx;
    int inb;

    if(curlev == 0) inb = 0;
    else inb = btab_p - 1;
    /* need to check */
    dx = btab[inb]->dx;
    j = btab[inb]->last;
    l = j;
    tab[0]->name = name;
    while(strcmp(tab[j]->name, name) != 0){
        j = tab[j]->link;
    }
    if(j != 0){
        error(id_redel);
        return 0;
    }
    /* insert new item */
    tab[tab_p] = (struct SymItem *)malloc(sizeof(struct SymItem));
    tab[tab_p]->name = name;
    tab[tab_p]->kind = kind;
    tab[tab_p]->type = type;
    tab[tab_p]->lev = curlev;
    /* constant do not need space */
    if(kind == constant) tab[tab_p]->adr = adr;
    else{
        tab[tab_p]->adr = dx;
        dx += size;
    }
    tab[tab_p]->ref = ref;
    tab[tab_p]->link = l;
    tab[tab_p]->size = size;
    tab[tab_p]->paras = 0;
    tab[tab_p]->isTmp = 0;
    tab[tab_p]->num = varId++;
    tab[tab_p]->end = 0;
    tab[tab_p]->hasRet = 0;
    tab[tab_p]->count = 0;
    tab[tab_p]->reg = -1;
    tab[tab_p]->loop = -1;
    tab[tab_p]->dirty = 0;
    tab[tab_p]->isModified = 0;
    tab[tab_p]->creg = -1;
    tab[tab_p]->isMain = 0;
    /* debug */
    if(DEBUG){
        printf("[name: %s, kind: %s, type: %s, adr: %d, ref: %d, size: %d]\n", name, kindsMap[kind], typesMap[type], tab[tab_p]->adr, ref, size);
    }
    /* modify btab*/
    btab[inb]->last = tab_p;
    btab[inb]->dx = dx;
    tab_p++;
    if(tab_p >= TABMLX) fatal(2);
    return tab_p - 1;
}