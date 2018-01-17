#ifndef _SEMANTIC_H
#define _SEMANTIC_H

#define LOG 0
#define CONSTPOOLSIZE 1024

void programParse(void);
void funcdel(int ty, char *name, int isMain);
void vardel(struct Set *fsys, int ty, char *name);
void constdel(struct Set *fsys);
void parametersParse(struct Set *fsys);
void statementParse(struct Set *fsys);
void identInfer(struct Set *fsys);
void assignmentParse(struct Set *fsys, int i);
void statementSequenceParse(struct Set *fsys);
struct SymItem *arrSelector(struct Set *fsys);
struct SymItem *expressionParse(struct Set *fsys);
struct SymItem *termParse(struct Set *fsys);
struct SymItem *factorParse(struct Set *fsys);
struct SymItem *callParse(struct Set *fsys, int i);

#endif