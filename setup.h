#ifndef _SETUP_
#define _SETUP_

#define DEBUG 0
#define PRINTMIDCODE 0
#define MIPSCONSOLE 0

#define GLOBALREGALLOCFLAG 0
#define COUNTASSIGNREG 1
#define COMMONSUNEXPRE 1
#define CONSTANTFOLDING 1
#define VARIABLEFOLDING 1

enum symbols{
    beginsy,
    intcon, charcon, strcon, //constant
    plus, minus, times, idiv, lss, leq, gtr, geq, neq, eql, //calculate
    lparent, rparent, lbrack, rbrack, comma, semicolon, colon, becomes, lbrace, rbrace, //map symbols
    ifsy, elsesy, whilesy, switchsy, casesy, defaultsy, returnsy, //keywords
    intsy, charsy, constsy, voidsy, mainsy, //type
    ident, printfsy, scanfsy,
    endsy //read end symbol
};

enum types{
    ints, chars, voids, strings, label
};

enum kinds{
    constant, variable, function, array, parameter
};

extern char symbolMap[64][20];
extern char strMap[64][20];
extern char kindsMap[5][10];
extern char typesMap[5][10];

extern char keys[20][20];
extern int ksys[20];
extern char sps[130];
extern int nkw; 
extern FILE *srcfp;
extern FILE *asmfp;
extern FILE *midfp;
extern int isGenAsm;
extern int isGenMid;

extern struct Set progbegsys;
extern struct Set factbegsys;
extern struct Set statbegsys;
extern struct Set funcbegsys;

void setup();

#endif