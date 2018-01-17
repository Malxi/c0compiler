#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "set.h"
#include "symbol.h"
#include "setup.h"
#include "error.h"
#include "semantic.h"
#include "midcode.h"
#include "mipsGen.h"
#include "optimizer.h"
#include "table.h"


void test_set(){
    struct Set A;
    s_init(&A);
    s_insert(&A, 10);
    s_insert(&A, 20);
    s_insert(&A, 20);
    s_print(&A);
    printf("res: %d, expected: %d\n", s_has(&A, 20), 1);
    s_erase(&A, 20);
    printf("res: %d, expected: %d\n", s_has(&A, 20), 0);

    struct Set B;
    s_init(&B);
    s_insert(&B, 10);
    s_insert(&B, 30);
    s_insert(&B, 20);
    s_print(&B);
    printf("res: %d, expected: %d\n", s_has(&B, 20), 1);
    s_erase(&B, 20);
    printf("res: %d, expected: %d\n", s_has(&B, 20), 0);

    s_plus(&A, &B);
    s_print(&A);

    s_insert(&A, 0);
    s_minus(&A, &B);
    s_print(&A);
}

void test_symbol(){
    int i = 1;
    while(sy != endsy){
        //printf("line: %d, col: %d ", line, np);
        printf("%d ", i);
        if(sy == ident){
            printf("%s ", symbolMap[sy]);
            printf("%s\n", id);
        }else if(sy == intcon){
            printf("%s ", symbolMap[sy]);
            printf("%d\n", inum);
        }else if(sy == charcon){
            printf("%s ", symbolMap[sy]);
            printf("%c\n", inum);
        }else if(sy == strcon){
            printf("%s ", symbolMap[sy]);
            printf("%s\n", strbuf);
        }else{
            printf("%s ", symbolMap[sy]);
            printf("%s\n", strMap[sy]);
        }
        nextsy();
        i++;
    }
}

void test_CFG(struct SymItem *func){
    genCFG(func);
}

void test_liveinter(struct SymItem *func){
    genLiveIntervals(func);
}


int main(int argc,char** argv) {
    setup();
    char srcfpName[100];
    char *filename;
    if(argc == 1){
        printf("Input file name: \n");
        scanf("%s", srcfpName);
        filename = srcfpName;
    }else{
       filename = argv[1]; 
    }
    if((srcfp = fopen(filename, "r")) == NULL){
        //printf("Open file fialed");
        fatal(0);
        exit(0);
    }
    if((asmfp = fopen("out.asm", "w")) == NULL){
        //printf("Open file fialed");
        fatal(1);
        exit(0);
    }
    if((midfp = fopen("midcode.txt", "w")) == NULL){
        //printf("Open file fialed");
        fatal(1);
        exit(0);
    }
    isGenAsm = 1;
    isGenMid = 1;
    //nextsy();
    //test_symbol();
    programParse();
    outputMcode();
    /* op */
    fclose(midfp);
    if((midfp = fopen("op_midcode.txt", "w")) == NULL){
        //printf("Open file fialed");
        fatal(1);
        exit(0);
    }
    if(COMMONSUNEXPRE){
        genCSE();
        copyOPMcode();
        isGenMid = 1;
        outputMcode();
    }
    if(s_isEmpty(&Errset)){
        genMips();           
    }
    fclose(srcfp);
    fclose(asmfp);
    fclose(midfp);
    //system("pause");
    return 0;
}