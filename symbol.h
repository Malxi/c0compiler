#ifndef _SYMBOL_H
#define _SYMBOL_H

#define IDLMX 100

#define NUMLMX 10
#define NUMMAX 2147483647
#define NUMMIN -2147483648

#define STRLMX 200
#define BUFLNX 1024

#define TABSTEP 8

extern enum symbols sy;
extern char ch;
extern char id[IDLMX];
extern int inum; 
extern char strbuf[BUFLNX];
extern int line;
extern char linebuf[BUFLNX];
extern int np;
extern int errpos;
extern int cc;

void nextsy(void);
char *getName(void);
char *getStrcon(void);

#endif