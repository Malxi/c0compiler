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

unsigned int sp = 0x00002ffc;
int tdx = 0;
struct SymBlock *gloBlock; 
struct SymBlock *locBlock;
struct SymItem *func;

char mipsInsMap[60][10] = {
    "add", "addi", "addiu", "addu", "sub", "subu",
    "and", "andi", "nor", "or", "ori", "xor", "xori",
    "sll", "sllv", "sra", "srav", "srl", "srlv",
    "div", "divu", "mul", "mult", "multu", "mfhi", "mflo", 
    "slt", "slti", "sltiu", "sltu",
    "j", "jal", "jalr", "jr",
    "beq", "bgez", "bgtz", "blez", "bltz", "bne",
    "lb", "lbu", "lh", "lhu", "lui", "lw",
    "sb", "sh", "sw",
    "syscall", "nop",
    "li"
};

char mipsRegMap[32][10] = {
    "$zero", "$at", "$v0", "$v1",
    "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9",
    "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra"
};

struct SymItem *tVarStack[1000];
int tVar_p = 0;

void genLabel(struct FCode *c);
void genCal_r(int ins, int rd, int rs, int rt);
void genCal_i(int ins, int rt, int rs, int imm);
void genCal_s(int ins, int rd, int rt, int s);
void genLd(int ins, int rt, int base, int offset);
void genSt(int ins, int rt, int base, int offset);
void genSyscall();
void genAllocate(int size);
void genFree();


void genLabel(struct FCode *c){
    struct SymItem *res;
    res = c->res;

    if(MIPSCONSOLE) printf("label_%s:\n", res->name);
    if(isGenAsm) fprintf(asmfp, "label_%s:\n", res->name);
}

void genSyscall(int ins, int v, int args, int a0, int a1){
    if(args > 0) genCal_i(LI, reg_a0, 0, a0);
    if(args > 1) genCal_i(LI, reg_a0, 0, a1);
    genCal_i(LI, reg_v0, 0, v);
    if(MIPSCONSOLE) printf("\tsyscall\n");
    if(isGenAsm) fprintf(asmfp, "\tsyscall\n");
}

void genCal_r(int ins, int rd, int rs, int rt){
    if(MIPSCONSOLE) printf("\t%s %s %s %s\n", mipsInsMap[ins], mipsRegMap[rd],  mipsRegMap[rs],  mipsRegMap[rt]);
    if(isGenAsm) fprintf(asmfp, "\t%s %s %s %s\n", mipsInsMap[ins], mipsRegMap[rd],  mipsRegMap[rs],  mipsRegMap[rt]);
}

void genCal_i(int ins, int rt, int rs, int imm){
    if(ins == LUI || ins == LI){
        if(MIPSCONSOLE) printf("\t%s %s %d\n", mipsInsMap[ins], mipsRegMap[rt], imm);
        if(isGenAsm) fprintf(asmfp, "\t%s %s %d\n", mipsInsMap[ins], mipsRegMap[rt], imm);
    }else{
        if(MIPSCONSOLE) printf("\t%s %s %s %d\n", mipsInsMap[ins], mipsRegMap[rt], mipsRegMap[rs], imm);
        if(isGenAsm) fprintf(asmfp, "\t%s %s %s %d\n", mipsInsMap[ins], mipsRegMap[rt], mipsRegMap[rs], imm);
    }
}

void genCal_s(int ins, int rd, int rt, int s){
    if(MIPSCONSOLE) printf("\t%s %s %s %d\n", mipsInsMap[ins], mipsRegMap[rd], mipsRegMap[rt], s);
    if(isGenAsm) fprintf(asmfp, "\t%s %s %s %d\n", mipsInsMap[ins], mipsRegMap[rd], mipsRegMap[rt], s);
}

void genMD(int ins, int rd, int rs, int rt){
    if(MIPSCONSOLE) printf("\t%s %s %s\n", mipsInsMap[ins], mipsRegMap[rs], mipsRegMap[rt]);
    if(isGenAsm) fprintf(asmfp, "\t%s %s %s\n", mipsInsMap[ins], mipsRegMap[rs], mipsRegMap[rt]);
    if(ins == MULTU || ins == MULT) ins = MFLO;
    else ins = MFLO;
    if(MIPSCONSOLE) printf("\t%s %s\n", mipsInsMap[ins], mipsRegMap[rd]);
    if(isGenAsm) fprintf(asmfp, "\t%s %s\n", mipsInsMap[ins], mipsRegMap[rd]);
}

void genLd(int ins, int rt, int base, int offset){
    if(MIPSCONSOLE) printf("\t%s %s %d(%s)\n", mipsInsMap[ins], mipsRegMap[rt], offset*4, mipsRegMap[base]);
    if(isGenAsm) fprintf(asmfp, "\t%s %s %d(%s)\n", mipsInsMap[ins], mipsRegMap[rt], offset*4, mipsRegMap[base]);
}

void genSt(int ins, int rt, int base, int offset){
    if(MIPSCONSOLE) printf("\t%s %s %d(%s)\n", mipsInsMap[ins], mipsRegMap[rt], offset*4, mipsRegMap[base]);
    if(isGenAsm) fprintf(asmfp, "\t%s %s %d(%s)\n", mipsInsMap[ins], mipsRegMap[rt], offset*4, mipsRegMap[base]);
}

void genJump(int ins, int rs, char *label){
    if(ins == JAL || ins == J){
        if(MIPSCONSOLE) printf("\t%s label_%s\n", mipsInsMap[ins], label);
        if(isGenAsm) fprintf(asmfp, "\t%s label_%s\n", mipsInsMap[ins], label);
    }else if(ins == JR){
        if(MIPSCONSOLE) printf("\t%s %s\n", mipsInsMap[ins], mipsRegMap[rs]);
        if(isGenAsm) fprintf(asmfp, "\t%s %s\n", mipsInsMap[ins], mipsRegMap[rs]);
    }else{
        printf("Invalid jump instruction\n");
    }
    if(MIPSCONSOLE) printf("\t%s\n", mipsInsMap[NOP]);
    if(isGenAsm) fprintf(asmfp, "\t%s\n", mipsInsMap[NOP]);
}

void genBranch(int ins, int rs, int rt, char *label){
    if(ins == BEQ || ins == BNE){
        if(MIPSCONSOLE) printf("\t%s %s %s label_%s\n", mipsInsMap[ins], mipsRegMap[rs], mipsRegMap[rt], label);
        if(isGenAsm) fprintf(asmfp, "\t%s %s %s label_%s\n", mipsInsMap[ins], mipsRegMap[rs], mipsRegMap[rt], label);
    }
    else{
        if(MIPSCONSOLE) printf("\t%s %s label_%s\n", mipsInsMap[ins], mipsRegMap[rs], label);
        if(isGenAsm) fprintf(asmfp, "\t%s %s label_%s\n", mipsInsMap[ins], mipsRegMap[rs], label);
    }
    if(MIPSCONSOLE) printf("\t%s\n", mipsInsMap[NOP]);
    if(isGenAsm) fprintf(asmfp, "\t%s\n", mipsInsMap[NOP]);
}

void genAllocate(int size){
    int imm;
    
    imm = -size * 4;
    sp += imm;
    if(imm > IMM_MAX || imm < IMM_MIN){
        genCal_r(OR, reg_t7, reg_zero, reg_zero);
        genCal_i(LUI, reg_t7, 0, size>>16);
        genCal_i(ORI, reg_t7, reg_t7, size&0x0000ffff);
        genCal_r(SUBU, reg_sp, reg_sp, reg_t7);
    }else{
        genCal_i(ADDIU, reg_sp, reg_sp, imm);
    }
}

void genFree(int size){
    int imm;
    
    imm = size * 4;
    sp += imm;
    if(imm > IMM_MAX || imm < IMM_MIN){
        genCal_r(OR, reg_t7, reg_zero, reg_zero);
        genCal_i(LUI, reg_t7, 0, size>>16);
        genCal_i(ORI, reg_t7, reg_t7, size&0x0000ffff);
        genCal_r(ADDU, reg_sp, reg_sp, reg_t7);
    }else{
        genCal_i(ADDIU, reg_sp, reg_sp, imm);
    }
}

void genGlobalAllocate(int size){
    genAllocate(size);
    genCal_r(OR, reg_gp, reg_zero, reg_sp);
}

void allocateTmp(struct SymItem *tmp){
    int i;

    for(i = 0; i < tVar_p; i++){
        if(tVarStack[i] == tmp){
            return;
        }
    }
    if(tVarStack[tVar_p] == 0){
        tmp->adr = locBlock->dx;
        tVarStack[tVar_p++] = tmp;
        locBlock->dx++;
        tdx++;
    }else{
        tmp->adr = locBlock->dx;
        tVarStack[tVar_p++] = tmp;
        locBlock->dx++;
        tdx++;
    }
}

void genLocalAllocate(int size){
    genAllocate(size);
    //genCal_r(OR, reg_fp, reg_zero, reg_sp);
}

void genTmpAllocate(struct SymItem *func){
    int cd;
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    struct FCode *c;

    tdx = 0;

    for(cd = func->ref; cd < func->end; cd++){
        c = mCode[cd];
        a = c->a;
        b = c->b;
        res = c->res;
        if(a != 0 && a->kind != constant && a->isTmp){
            allocateTmp(a);
        }
        if(b != 0 && b->kind != constant && b->isTmp){
            allocateTmp(b);
        }
        if(res != 0 && res->kind != constant && res->isTmp){
            allocateTmp(res);
        }
    }
    //genAllocate(tdx);
}

void loadTmp(int rt, struct SymItem *tmp){
    int base, offset;

    base = reg_sp;
    offset = tmp->adr;

    genLd(LW, rt, base, offset);
} 

void storeTmp(int rt, struct SymItem *tmp){
    int base, offset;

    base = reg_sp;
    offset = tmp->adr;

    genSt(SW, rt, base, offset);
}

void loadVar(int rt, struct SymItem *var){
    int base, offset;

    offset = var->adr;

    if(var->lev == 0){
        genLd(LW, rt, reg_gp, offset);
    }else{
        if(var->kind == parameter) genLd(LW, rt, reg_fp, offset);
        else genLd(LW, rt, reg_sp, offset);
    }
}

void storeVar(int rt, struct SymItem *var){
    int base, offset;

    offset = var->adr;

    if(var->lev == 0){
        genSt(SW, rt, reg_gp, offset);
    }else{
        if(var->kind == parameter) genLd(SW, rt, reg_fp, offset);
        else genSt(SW, rt, reg_sp, offset);
    }
}

void loadConstant(int rt, struct SymItem *con){
    int imm;
    int sign;

    /*
    if(con->adr >= 0){
        imm = con->adr;
        sign = 1;
    }else{
        imm = -con->adr;
        sign = -1;
    }
    if(con->adr > IMM_MAX || con->adr < IMM_MIN){
        genCal_r(OR, rt, reg_zero, reg_zero);
        genCal_i(LUI, rt, reg_zero, imm>>16);
        genCal_i(ORI, rt, rt, imm&0x0000ffff);
        if(sign == -1) genCal_r(SUBU, rt, reg_zero, rt);
    }else{
        genCal_i(ADDIU, rt, reg_zero, con->adr);
    }
    */
    genCal_i(LI, rt, reg_zero, con->adr);
}

void loadIdent(int reg, struct SymItem *vct){
    if(vct->kind == constant){
        loadConstant(reg, vct);
    }else if(vct->isTmp){
        loadTmp(reg, vct);
    }else{
        loadVar(reg, vct);
    }
}

void storeIdent(int reg, struct SymItem *vct){
    if(vct->isTmp){
        storeTmp(reg, vct);
    }else{
        storeVar(reg, vct);
    }
}

void genPrologue(struct SymItem *func){
    /* allocate the tmp stack */
    genTmpAllocate(func);
    if(MIPSCONSOLE) printf("\t# start of prologue\n");
    if(isGenAsm) fprintf(asmfp, "\t# start of prologue\n");
    genAllocate(locBlock->dx);
    if(MIPSCONSOLE) printf("\t# end of prologue\n");
    if(isGenAsm) fprintf(asmfp, "\t# end of prologue\n");
}

void genCalleePopFrame(){
    genLd(LW, reg_ra, reg_sp, frame_RA);
    genLd(LW, reg_fp, reg_sp, frame_FP);
    //genLd(LW, reg_s0, reg_sp, frame_S0);
    //genLd(LW, reg_s1, reg_sp, frame_S1);
    //genLd(LW, reg_s2, reg_sp, frame_S2);
    //genLd(LW, reg_s3, reg_sp, frame_S3);
    //genLd(LW, reg_s4, reg_sp, frame_S4);
    //genLd(LW, reg_s5, reg_sp, frame_S5);
    //genLd(LW, reg_s6, reg_sp, frame_S6);
    //genLd(LW, reg_s7, reg_sp, frame_S7);
}

void genCalleeSaveFrame(){
    genLd(SW, reg_ra, reg_sp, frame_RA);
    genLd(SW, reg_fp, reg_sp, frame_FP);
    //genLd(SW, reg_s0, reg_sp, frame_S0);
    //genLd(SW, reg_s1, reg_sp, frame_S1);
    //genLd(SW, reg_s2, reg_sp, frame_S2);
    //genLd(SW, reg_s3, reg_sp, frame_S3);
    //genLd(SW, reg_s4, reg_sp, frame_S4);
    //genLd(SW, reg_s5, reg_sp, frame_S5);
    //genLd(SW, reg_s6, reg_sp, frame_S6);
    //genLd(SW, reg_s7, reg_sp, frame_S7);
}

void genEpilogue(){
    if(MIPSCONSOLE) printf("\t# start of epilogue\n");
    if(isGenAsm) fprintf(asmfp, "\t# start of epilogue\n");
    genFree(locBlock->dx);
    if(MIPSCONSOLE) printf("\t# end of epilogue\n");
    if(isGenAsm) fprintf(asmfp, "\t# end of epilogue\n");
}

void genDataSeg(){
    int i;

    if(MIPSCONSOLE) printf(".data\n");
    if(isGenAsm) fprintf(asmfp, ".data\n");
    /* allocate the global stack */
    if(MIPSCONSOLE) printf("\t#global allocate\n");
    if(isGenAsm) fprintf(asmfp, "\t#global allocate\n");
    //genGlobalAllocate(gloBlock->dx);
    if(4 * gloBlock->dx > 0x3000){
        printf("error: data size is too big\n");
        exit(0);
    }
    if(MIPSCONSOLE) printf("\tglobal: .space %d\n", 4 * gloBlock->dx);
    if(isGenAsm) fprintf(asmfp, "\tglobal: .space %d\n", 4 * gloBlock->dx);
    for(i = 0; i < stab_p; i++){
        if(MIPSCONSOLE) printf("\tstr_%d: ", i);
        if(MIPSCONSOLE) printf(".asciiz \"");
        if(MIPSCONSOLE) printf("%s\"\n", stab[i]);
        if(isGenAsm){
            fprintf(asmfp, "\tstr_%d: ", i);
            fprintf(asmfp, ".asciiz \"");
            fprintf(asmfp, "%s\"\n", stab[i]);
        }
    }
}

void genProgStart(){
    if(MIPSCONSOLE) printf(".text\n");
    if(isGenAsm) fprintf(asmfp, ".text\n");
    if(MIPSCONSOLE) printf("\tla $gp global\n");
    if(isGenAsm) fprintf(asmfp, "\tla $gp global\n");
    //genCal_r(OR, reg_gp, reg_zero, reg_zero);
    genJump(JAL, 0, "main");
    genSyscall(SYSCALL, 17, 1, 0, 0);
}

void genCal(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;
    
    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    rs = reg_zero;
    rt = reg_zero;
    rd = reg_zero;
    /* deal with a, just load to t1*/
    if(a == 0){
        rs = reg_zero;
    }else{
        loadIdent(reg_t1, a);
        rs = reg_t1;
    }
    /* deal with b, just load to t2*/
    if(b == 0){
        rt = reg_zero;
    }else{
        loadIdent(reg_t2, b);
        rt = reg_t2;
    }
    /* generate cal code */
    switch(op){
        case minus: genCal_r(SUBU, reg_t0, rs, rt);break;
        case plus: genCal_r(ADDU, reg_t0, rs, rt);break;
        case times: genCal_r(MUL, reg_t0, rs, rt);break;
        case idiv: genCal_r(DIV, reg_t0, rs, rt);break;
    }
    storeIdent(reg_t0, res);
}

void genCpy(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    rd = reg_t0;
    rt = reg_zero;
    rs = reg_t1;

    loadIdent(rs, a);
    genCal_r(OR, rd, rs, rt);
    storeIdent(rd, res);
}

void genLdAry(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    loadIdent(reg_t1, b);
    genCal_s(SLL, reg_t1, reg_t1, 2);
    if(a->lev == 0){
        genCal_r(ADDU, reg_t2, reg_t1, reg_gp);        
    }else{
        genCal_r(ADDU, reg_t2, reg_t1, reg_sp);
    }
    genLd(LW, reg_t0, reg_t2, a->adr);
    storeIdent(reg_t0, res);
}

void genStAry(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    loadIdent(reg_t1, b);
    genCal_s(SLL, reg_t1, reg_t1, 2);
    if(res->lev == 0){
        genCal_r(ADDU, reg_t2, reg_t1, reg_gp);        
    }else{
        genCal_r(ADDU, reg_t2, reg_t1, reg_sp);
    }
    loadIdent(reg_t0, a);
    genSt(SW, reg_t0, reg_t2, res->adr);
}

void genGoto(struct FCode *c){
    struct SymItem *res;

    res = c->res;
    genJump(J, 0, res->name);
}

void genGoFalse(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    
    if(b != 0){
        loadIdent(reg_t1, a);
        loadIdent(reg_t2, b);
        genCal_r(SUBU, reg_t0, reg_t1, reg_t2);
    }else{
        loadIdent(reg_t0, a);
    }

    switch(op){
        case lss: genBranch(BGEZ, reg_t0, reg_zero, res->name);break;
        case leq: genBranch(BGTZ, reg_t0, reg_zero, res->name);break;
        case gtr: genBranch(BLEZ, reg_t0, reg_zero, res->name);break;
        case geq: genBranch(BLTZ, reg_t0, reg_zero, res->name);break;
        case neq: genBranch(BEQ, reg_t0, reg_zero, res->name);break;
        case eql: genBranch(BNE, reg_t0, reg_zero, res->name);break;
    }
}

void genRet(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    
    if(res != 0) loadIdent(reg_v1, res);
    genCalleePopFrame();
    genEpilogue();
    genJump(JR, reg_ra, 0);
}

void genCall(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int count;
    int i;
    int offset;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    offset = -1;
    for(i = count - 1; i >= 0; i--){
        loadIdent(reg_t0, c->paraList[i]);
        genSt(SW, reg_t0, reg_sp, offset);
        offset -= 1;
    }
    if(count > 0) genAllocate(count);
    genCal_r(OR, reg_fp, reg_sp, reg_zero);
    genJump(JAL, 0, a->name);
    if(count > 0) genFree(count);
    genLd(LW, reg_fp, reg_sp, frame_FP);
    if(a->type != voids) storeIdent(reg_v1, res);
}

void genPrintf(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int count;
    int i;
    int offset;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    for(i = 0; i < count; i++){
        switch(c->paraList[i]->type){
            case ints:{
                genCal_i(LI, reg_v0, 0, 1);
                loadIdent(reg_a0, c->paraList[i]);
                break; 
            }
            case chars:{
                genCal_i(LI, reg_v0, 0, 11);
                loadIdent(reg_a0, c->paraList[i]);  
                break; 
            }
            case strings:{
                genCal_i(LI, reg_v0, 0, 4);
                if(MIPSCONSOLE) printf("\tla $a0 str_%d\n", c->paraList[i]->ref);
                if(isGenAsm) fprintf(asmfp, "\tla $a0 str_%d\n", c->paraList[i]->ref);
                break; 
            }
        }
        if(MIPSCONSOLE) printf("\tsyscall\n");
        if(isGenAsm) fprintf(asmfp, "\tsyscall\n");
    }
}

void genScanf(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int count;
    int i;
    int offset;
    int v;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    for(i = 0; i < count; i++){
        switch(c->paraList[i]->type){
            case ints:{
                //genCal_i(LI, reg_v0, 0, 5);
                v = 5;
                break; 
            }
            case chars:{
                //genCal_i(LI, reg_v0, 0, 12);
                v = 12;
                break; 
            }
        }
        genSyscall(SYSCALL, v, 0, 0, 0);
        storeIdent(reg_v0, c->paraList[i]);
    }
}

void genMips(){
    int index;
    int cd;
    struct FCode *c;

    index = 0;
    gloBlock = btab[0];
    /* data segment */
    genDataSeg();
    
    //genAllocate(0x40000);

    /* gen jal main */
    genProgStart();
    
    /* generate mips code for function */
    for(index = 1; index < btab_p; index++){
        locBlock = btab[index];
        func = tab[locBlock->cur];
        cd = func->ref;
        genLabel(mCode[cd]);
        genPrologue(func);
        genCalleeSaveFrame();
        for(cd = func->ref + 1; cd < func->end; cd++){
            c = mCode[cd];
            switch(c->type){
                case mid_label: genLabel(c);break;
                case mid_cal: genCal(c);break;
                case mid_cpy: genCpy(c);break;
                case mid_ldAry: genLdAry(c);break;
                case mid_stAry: genStAry(c);break;
                case mid_goto: genGoto(c);break;
                case mid_goFalse: genGoFalse(c);break;
                case mid_ret: genRet(c);break;
                case mid_call: genCall(c);break;
                case mid_printf: genPrintf(c);break;
                case mid_scanf: genScanf(c);break;
            }
        }
        if(func->type == voids){
            genCalleePopFrame();
            genEpilogue();
            genJump(JR, reg_ra, 0);
        }
    }
}