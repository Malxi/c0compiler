#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "error.h"
#include "setup.h"
#include "table.h"

char symbolMap[64][20] = {
    "beginsy", "intcon", "charcon", "strcon", "plus", "minus", "times", "idiv", "lss", 
    "leq", "gtr", "geq", "neq", "eql", "lparent", "rparent", "lbrack", "rbrack", "comma", 
    "semicolon", "colon", "becomes", "lbrace", "rbrace", "ifsy", "elsesy", "whilesy", "switchsy", "casesy", 
    "defaultsy", "returnsy", "intsy", "charsy", "constsy","voidsy", "mainsy","ident", "printfsy", "scanfsy", "endsy"
};

char strMap[64][20] = {
    "beginsy", "intcon", "charcon", "strcon", "+", "-", "*", "/", "<", 
    "<=", ">", ">=", "!=", "==", "(", ")", "[", "]", ",", 
    ";", ":", "=", "{", "}", "if", "else", "while", "switch", "case", 
    "default", "return", "int", "char", "const", "void", "main", "ident", "printf", "scanf", "endsy"
};

char kindsMap[5][10] = {
    "constant",
    "variable",
    "function",
    "array",
    "parameter"
};

char typesMap[5][10] = {
    "ints",
    "chars",
    "voids",
    "label"
};

int nkw = 14;
char keys[20][20] = {"case", "char", "const", "default", "else", "if", "int", "main", "printf", "return", "scanf", "switch", "void", "while"};
int ksys[20] = {0};
char sps[130] = {0};
FILE *srcfp = 0;
FILE *asmfp = 0;
FILE *midfp = 0;
int isGenAsm = 0;
int isGenMid = 0;

struct Set progbegsys;
struct Set funcbegsys;
struct Set factbegsys;
struct Set statbegsys;

void setup(){
    ksys[0] = casesy;
    ksys[1] = charsy;
    ksys[2] = constsy;
    ksys[3] = defaultsy;
    ksys[4] = elsesy;
    ksys[5] = ifsy;
    ksys[6] = intsy;
    ksys[7] = mainsy;
    ksys[8] = printfsy;
    ksys[9] = returnsy;
    ksys[10] = scanfsy;
    ksys[11] = switchsy;
    ksys[12] = voidsy;
    ksys[13] = whilesy;

    sps['+'] = plus;
    sps['-'] = minus;
    sps['/'] = idiv;
    sps['*'] = times;
    sps['('] = lparent;
    sps[')'] = rparent;
    sps['<'] = lss;
    sps['>'] = gtr;
    sps['='] = becomes;
    sps[':'] = colon;
    sps[';'] = semicolon;
    sps[','] = comma;
    sps['['] = lbrack;
    sps[']'] = rbrack;
    sps['{'] = lbrace;
    sps['}'] = rbrace;

    s_init(&Errset);
    s_init(&progbegsys);
    s_insert(&progbegsys, constsy);
    s_insert(&progbegsys, intsy);
    s_insert(&progbegsys, charsy);
    s_insert(&progbegsys, voidsy);
    s_insert(&progbegsys, endsy);
    s_init(&factbegsys);
    s_insert(&factbegsys, plus);
    s_insert(&factbegsys, minus);
    s_insert(&factbegsys, intcon);
    s_insert(&factbegsys, charcon);
    s_insert(&factbegsys, ident);
    s_insert(&factbegsys, lparent);
    s_insert(&factbegsys, endsy);
    s_init(&statbegsys);
    s_insert(&statbegsys, lbrace);
    s_insert(&statbegsys, ifsy);
    s_insert(&statbegsys, whilesy);
    s_insert(&statbegsys, switchsy);
    s_insert(&statbegsys, returnsy);
    s_insert(&statbegsys, scanfsy);
    s_insert(&statbegsys, printfsy);
    s_insert(&statbegsys, semicolon);
    s_insert(&statbegsys, casesy);
    s_insert(&statbegsys, defaultsy);
    s_insert(&statbegsys, ident);
    s_insert(&statbegsys, elsesy);
    s_init(&funcbegsys);
    s_insert(&funcbegsys, constsy);
    s_insert(&funcbegsys, intsy);
    s_insert(&funcbegsys, charsy);


    tab[0] = (struct SymItem *)malloc(sizeof(struct SymItem));
    tab[0]->name = NULL;
    tab[0]->kind = -1;
    tab[0]->type = -1;
    tab[0]->lev = -1;
    tab[0]->adr = -1;
    tab[0]->ref = -1;
    tab[0]->link = -1;
    tab[0]->size = -1;
}