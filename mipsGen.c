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
#include "optimizer.h"

int tdx = 0;
struct SymBlock *gloBlock; 
struct SymBlock *locBlock;
struct SymItem *func;
int curBB = -1;
int cd;

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
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$t8", "$t9",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$k0", "$k1",
    "$gp", "$sp", "$fp", "$ra"
};

struct SymItem *tVarStack[1000];
int tVar_p = 0;

struct SymItem *regPool[REGPOOLSIZE] = {0};
int globalRegSaveMap[32] = {0};

void genLabel(struct FCode *c);
void genCal_r(int ins, int rd, int rs, int rt);
void genCal_i(int ins, int rt, int rs, int imm);
void genCal_s(int ins, int rd, int rt, int s);
void genLd(int ins, int rt, int base, int offset);
void genSt(int ins, int rt, int base, int offset);
void genSyscall();
void genAllocate(int size);
void genFree();
void storeIdent(int reg, struct SymItem *vct);
void loadIdent(int reg, struct SymItem *vct);

void allocGlobalReg(){
    int i;
    int reg;
    struct SymItem *vct;
    int vec;
    /* alloc global reg for func */
    for(i = 0; i < colorp; i++){
        vec = colorSquence[i];
        vec = RIG_vec2hash[vec];
        reg = hashTab[vec]->reg;
        if(regPool[reg] != 0){
            printf("global pool is not empty!\n");
        }
        if(hashTab[vec]->lev == 0){
            printf("global variable is assiged register!\n");
        }
        //regPool[reg] = 1; //hashTab[vec];
        globalRegSaveMap[reg] = 1;
    } 
}

void allocRegWithCount(){
    int i;
    int reg;
    struct SymItem *vct;

    /* alloc global reg for func */
    for(i = 0; i < GLOBALREQ && i < count_p; i++){
        vct = countTab[i];
        reg = vct->creg;
        if(reg != -1){
            globalRegSaveMap[reg] = 1;
            regPool[reg] = vct;
        }
    }
}

void clearGlobalReg(int bb){
    int i;
    int reg;
    struct SymItem *vct;
    int vec;
    if(bb == 1) return;
    /* alloc global reg for bb block */
    if(MIPSCONSOLE) printf("\t#Base block %d clear gr start\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d clear gr start\n", curBB);
    /*
    for(i = 0; i < SETSIZE; i++){
        if(s_has(&l_in[bb], i)){
            vec = i;
            reg = hashTab[vec]->reg;
            if(hashTab[vec] == regPool[reg]) continue;
            if(regPool[reg] != 0 && regPool[reg]->dirty){
                storeIdent(reg, regPool[reg]);
                regPool[reg]->dirty = 0;
            }
            regPool[reg] = hashTab[vec];
            loadIdent(reg, hashTab[vec]);
        }
    }
    */
    /*
    for(i = reg_s0; i <= reg_s7; i++){
        if(regPool[i] != 0 && regPool[i]->dirty){
            storeIdent(i, regPool[i]);
        }
        regPool[i] = 0;
    }
    */
    /*
    for(i = reg_s0; i <= reg_s7; i++){
        vct = regPool[i];
        if(vct == 0) continue;
        vec = vct->num;
        if(!s_has(&l_out[bb], vec)){
            if(regPool[i] != 0 && regPool[i]->dirty){
                storeIdent(i, regPool[i]);
                regPool[i]->dirty = 0;
            }
            regPool[i] = 0;
        }
    }
    */
    /*
    for(i = reg_s0; i <= reg_s7; i++){
        if(regPool[i] != 0 && regPool[i]->dirty){
            storeIdent(i, regPool[i]);
            regPool[i]->dirty = 0;
        }
        regPool[i] = 0;
    }
    */
    if(MIPSCONSOLE) printf("\t#Base block %d clear gr end\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d clear gr end\n", curBB);
}

void loadGlobalReg(int bb){
    int i;
    int reg;
    struct SymItem *vct;
    int vec;
    /* alloc global reg for bb block */
    if(MIPSCONSOLE) printf("\t#Base block %d load gr start\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d load gr start\n", curBB);
    for(i = 0; i < SETSIZE; i++){
        if(s_has(&use[bb], i) || s_has(&def[bb], i)){
            vec = i;
            reg = hashTab[vec]->reg;
            if(reg == -1) continue;
            if(hashTab[vec] == regPool[reg]) continue;
            if(regPool[reg] != 0 && regPool[reg]->dirty){
                printf("s0 - s7 not clear\n");
                storeIdent(reg, regPool[reg]);
                regPool[reg]->dirty = 0;
            }
            if(reg < 0 || reg > 31){
                printf("error assign register\n");
            }
            if(!globalRegSaveMap[reg]){
                printf("error load to global register which has not been saved\n");
            }
            regPool[reg] = hashTab[vec];
            if(s_has(&use[bb], i)) loadIdent(reg, hashTab[vec]);
        }
    }
    if(MIPSCONSOLE) printf("\t#Base block %d load gr end\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d load gr end\n", curBB);
}

int isInPool(struct SymItem *vct){
    int i;
    for(i = 0; i <= 31; i++){
        if(regPool[i] == vct) return i;
    }
    return -1;
}

void mapReg(struct SymItem *vct, int reg){
    if(reg < reg_zero || reg > reg_ra){
        printf("map register error\n");
        return;
    }
    regPool[reg] = vct;
}

int allocReg(struct SymItem *var){
    int i, j;
    int use[32] = {0};
    int d;
    struct SymItem *vct;
    int kep1 = -1;
    int kep2 = -1;
    int type;
    int out;
    /* check reg and return its reg */
    /*
    if(var != 0 && var->reg != -1){
        if(regPool[var->reg] != 0 && regPool[var->reg] != var){
            if(regPool[var->reg] != 0 && regPool[var->reg]->dirty){
                storeIdent(var->reg, regPool[var->reg]);
            }
            //printf("confilict in global assign\n");
        }
        return var->reg;
    }
    */
    /* find a free register */
    for(i = reg_t0; i <= reg_t9; i++){
        if(regPool[i] == 0) return i;
    }
    /* spill */
    /* check current base block */
    for(i = reg_t0; i <= reg_t9; i++){
        d = cd;
        vct = regPool[i];
        if(vct->type == constant) use[i] -= 10;
        if(!vct->dirty) use[i] -= 5;
        while(d < func->end && curBB == mCode[d]->belong2BB){
            type = mCode[d]->type;
            if(vct == mCode[d]->a) use[i]++;
            if(vct == mCode[d]->b) use[i]++;
            if(vct == mCode[d]->res) use[i]++;
            if(type == mid_call || type == mid_printf || type == mid_scanf){
                for(j = 0; j < mCode[d]->count; j++){
                    if(vct == mCode[d]->paraList[j]) use[i]++;
                }
            }
            d++;
        }
    }
    /* find a register which need to out */
    /* check the register need to keep */
    for(i = reg_t0; i <= reg_t9; i++){
        if(regPool[i] == mCode[cd]->a) kep1 = i;
        if(regPool[i] == mCode[cd]->b) kep2 = i;
    }
    out = reg_t0;
    if(out == kep1 || out == kep2) out = reg_t1;
    if(out == kep1 || out == kep2) out = reg_t2;
    for(i = reg_t0; i <= reg_t9; i++){
        if(use[i] < use[out] && i != kep1 && i != kep2) out = i;
    }
    /* save i variable */
    if(regPool[out]->kind != constant) storeIdent(out, regPool[out]);
    regPool[out] = 0;
    return out;
}

void clearPool(){
    int i;
    if(MIPSCONSOLE) printf("\t#Base block %d clear start\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d clear start\n", curBB);
    for(i = reg_t0; i <= reg_t9; i++){
        if(regPool[i] != 0 && regPool[i]->dirty){//regPool[i]->kind != constant && !regPool[i]->isTmp){
            storeIdent(i, regPool[i]);
            regPool[i]->dirty = 0;
        }
        regPool[i] = 0;
    }
    if(MIPSCONSOLE) printf("\t#Base block %d clear end\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d clear end\n", curBB);
}

void savePool(){
    /* this for sy */
    int i;
    if(MIPSCONSOLE) printf("\t#Base block %d save start\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d save start\n", curBB);
    for(i = reg_t0; i <= reg_t9; i++){
        if(regPool[i] != 0 && regPool[i]->dirty){//regPool[i]->kind != constant){
            storeIdent(i, regPool[i]);
            regPool[i]->dirty = 0;
        }
    }
    if(MIPSCONSOLE) printf("\t#Base block %d save end\n", curBB);
    if(isGenAsm) fprintf(asmfp, "\t#Base block %d save end\n", curBB);
}

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
    //if(MIPSCONSOLE) printf("\t%s\n", mipsInsMap[NOP]);
    //if(isGenAsm) fprintf(asmfp, "\t%s\n", mipsInsMap[NOP]);
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
    //if(MIPSCONSOLE) printf("\t%s\n", mipsInsMap[NOP]);
    //if(isGenAsm) fprintf(asmfp, "\t%s\n", mipsInsMap[NOP]);
}

void genAllocate(int size){
    int imm;
    
    imm = -size * 4;
    //sp += imm;
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
    //sp += imm;
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
    if(vct->reg != -1 && reg != vct->reg && reg != reg_v1 && reg != reg_a0){
        if(DEBUG) printf("global assign register error!\n");
    }
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
    //genCal_r(OR, reg_fp, reg_sp, reg_zero);
    genAllocate(locBlock->dx);
    if(MIPSCONSOLE) printf("\t# end of prologue\n");
    if(isGenAsm) fprintf(asmfp, "\t# end of prologue\n");
}

void genCallerSaveReg(int count){
    int i;
    /* a0-a3 */
    if(MIPSCONSOLE) printf("\t# Caller save reg start\n");
    if(isGenAsm) fprintf(asmfp, "\t# Caller save reg start\n");
    for(i = reg_a0 + NOTPARAREGNUM; i < reg_a0 + NOTPARAREGNUM + PARAREGNUM && i < reg_a0 + NOTPARAREGNUM + count; i++){
        if(regPool[i] != 0 && regPool[i]->kind != constant){
            storeIdent(i, regPool[i]);
        }
    }
    if(MIPSCONSOLE) printf("\t# Caller save reg end\n");
    if(isGenAsm) fprintf(asmfp, "\t# Caller save reg end\n");
}

void genCallerPopReg(int count){
    int i;
    /* a0-a3 */
    if(MIPSCONSOLE) printf("\t# Caller pop reg start\n");
    if(isGenAsm) fprintf(asmfp, "\t# Caller pop reg start\n");
    for(i = reg_a0 + NOTPARAREGNUM; i < reg_a0 + NOTPARAREGNUM + PARAREGNUM && i < reg_a0 + NOTPARAREGNUM + count; i++){
        if(regPool[i] != 0){
            loadIdent(i, regPool[i]);
        }
    }
    if(MIPSCONSOLE) printf("\t# Caller pop reg end\n");
    if(isGenAsm) fprintf(asmfp, "\t# Caller pop reg end\n");
}

void genCalleeSaveFrame(){
    int i;
    /* s0-s7 if used and ra and fp */
    if(MIPSCONSOLE) printf("\t# Callee save frame start\n");
    if(isGenAsm) fprintf(asmfp, "\t# Callee save frame start\n");
    genSt(SW, reg_ra, reg_sp, frame_RA);
    genSt(SW, reg_fp, reg_sp, frame_FP);
    if(!func->isMain){
        for(i = reg_s0; i <= reg_s7; i++){
            if(globalRegSaveMap[i]){
                genSt(SW, i, reg_sp, frame_S0 + i - reg_s0);
            }
        }
    }
    if(MIPSCONSOLE) printf("\t# Callee save frame end\n");
    if(isGenAsm) fprintf(asmfp, "\t# Callee save frame end\n");
}

void genCalleePopFrame(){
    int i;
    //clearPool();
    if(MIPSCONSOLE) printf("\t# Callee pop frame start\n");
    if(isGenAsm) fprintf(asmfp, "\t# Callee pop frame start\n");
    genLd(LW, reg_ra, reg_sp, frame_RA);
    genLd(LW, reg_fp, reg_sp, frame_FP);
    for(i = reg_a0; i <= reg_a3; i++){
        if(regPool[i] != 0) regPool[i] = 0;
    }
    if(!func->isMain){
        for(i = reg_s0; i <= reg_s7; i++){
            if(globalRegSaveMap[i]){
                genLd(LW, i, reg_sp, frame_S0 + i - reg_s0);
            }
            globalRegSaveMap[i] = 0;
            /* global register values do not need to save */
            regPool[i] = 0;
        }
    }
    if(MIPSCONSOLE) printf("\t# Callee pop frame end\n");
    if(isGenAsm) fprintf(asmfp, "\t# Callee pop frame end\n");
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
    /* deal with a*/
    if(a == 0){
        rs = reg_zero;
    }else{
        rs = isInPool(a);
        if(rs == -1){
            rs = allocReg(a);
            mapReg(a, rs);
            loadIdent(rs, a);
        }
    }
    /* deal with b*/
    if(b == 0){
        rt = reg_zero;
    }else{
        rt = isInPool(b);
        if(rt == -1){
            rt = allocReg(b);
            mapReg(b, rt);
            loadIdent(rt, b);
        }
    }
    /* deal with res */
    rd = isInPool(res);
    if(rd == -1){
        rd = allocReg(res);
        mapReg(res, rd);
    }
    /* generate cal code */
    switch(op){
        case minus: genCal_r(SUBU, rd, rs, rt);break;
        case plus: genCal_r(ADDU, rd, rs, rt);break;
        case times: genCal_r(MUL, rd, rs, rt);break;
        case idiv: genCal_r(DIV, rd, rs, rt);break;
    }
    res->dirty = 1;
    //storeIdent(reg_t0, res);
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

    /* handel rd */
    rd = isInPool(res);
    if(rd == -1){
        rd = allocReg(res);
        mapReg(res, rd);
    }
    /* load a to rs */
    rs = isInPool(a);
    if(rs == -1){
        //rs = allocReg(a);
        //mapReg(a, rs);
        //loadIdent(rs, a);
        loadIdent(rd, a);
    }else{
        genCal_r(OR, rd, rs, rt);
    }
    //genCal_r(OR, rd, rs, rt);
    //storeIdent(rd, res);
    res->dirty = 1;
}

void genLdAry(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;
    int offset;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    /* res = a[b] */
    /* load b*/
    offset = isInPool(b);
    if(offset == -1){
        offset = allocReg(b);
        mapReg(b, offset);
        loadIdent(offset, b);
    }
    /* allocate a reg for res which will be covered*/
    rd = isInPool(res);
    if(rd == -1){
        rd = allocReg(res);
        mapReg(res, rd);
    }
    /* shfit b with res as temp reg */
    genCal_s(SLL, rd, offset, 2);
    if(a->lev == 0){
        genCal_r(ADDU, rd, rd, reg_gp);        
    }else{
        genCal_r(ADDU, rd, rd, reg_sp);
    }
    genLd(LW, rd, rd, a->adr);
    //storeIdent(reg_t0, res);
    res->dirty = 1;
}

void genStAry(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;
    int offset;

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    /* res[b] = a */
    /* load b to offset*/
    offset = isInPool(b);
    if(offset == -1){
        offset = allocReg(b);
        mapReg(b, offset);
        loadIdent(offset, b);
    }
    /* load a to rs*/
    rs = isInPool(a);
    if(rs == -1){
        rs = allocReg(a);
        mapReg(a, rs);
        loadIdent(rs, a);
    }
    /* alloc a temp reg rt to store offset shift */
    rt = allocReg(0);
    genCal_s(SLL, rt, offset, 2);
    if(res->lev == 0){
        genCal_r(ADDU, rt, rt, reg_gp);        
    }else{
        genCal_r(ADDU, rt, rt, reg_sp);
    }
    genSt(SW, rs, rt, res->adr);
}

void genGoto(struct FCode *c){
    struct SymItem *res;

    res = c->res;
    savePool();
    if(GLOBALREGALLOCFLAG) clearGlobalReg(curBB);
    if(GLOBALREGALLOCFLAG) loadGlobalReg(mCode[res->adr]->belong2BB);
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
        rs = isInPool(a);
        if(rs == -1){
            rs = allocReg(a);
            mapReg(a, rs);
            loadIdent(rs, a);
        }
        rt = isInPool(b);
        if(rt == -1){
            rt = allocReg(b);
            mapReg(b, rt);
            loadIdent(rt, b);
        }
        rd = allocReg(0);
        genCal_r(SUBU, rd, rs, rt);
    }else{
        rd = isInPool(a);
        if(rd == -1){
            rd = allocReg(a);
            mapReg(a, rd);
            loadIdent(rd, a);
        }
    }
    savePool();
    if(GLOBALREGALLOCFLAG) clearGlobalReg(curBB);
    if(GLOBALREGALLOCFLAG) loadGlobalReg(mCode[res->adr]->belong2BB);
    switch(op){
        case lss: genBranch(BGEZ, rd, reg_zero, res->name);break;
        case leq: genBranch(BGTZ, rd, reg_zero, res->name);break;
        case gtr: genBranch(BLEZ, rd, reg_zero, res->name);break;
        case geq: genBranch(BLTZ, rd, reg_zero, res->name);break;
        case neq: genBranch(BEQ, rd, reg_zero, res->name);break;
        case eql: genBranch(BNE, rd, reg_zero, res->name);break;
    }
}

void genRet(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int type;
    int rs, rt, rd;
    char funcExit[IDLMX] = {0};

    type = c->type;
    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;

    
    if(res != 0){
        rs = isInPool(res);
        if(rs == -1){
            loadIdent(reg_v1, res);
        }else{
            genCal_r(OR, reg_v1, rs, reg_zero);
        }
    }
    //genCalleePopFrame();
    //genEpilogue();
    //genJump(JR, reg_ra, 0);
    clearPool();
    if(MIPSCONSOLE) printf("%s_end", func->name);
    if(isGenAsm) sprintf(funcExit, "%s_end", func->name);
    genJump(J, 0, funcExit);
}

void genCall(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    int op;
    int count;
    int i;
    int offset;
    int p;
    int rd;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    offset = -1;
    /* load parameters */
    for(i = count - 1; i >= PARAREGNUM; i--){
        p = isInPool(c->paraList[i]);
        if(p == -1){
            p = allocReg(c->paraList[i]);
            mapReg(c->paraList[i], p);
            loadIdent(p, c->paraList[i]);
        }
        genSt(SW, p, reg_sp, offset);
        offset -= 1;
    }
    /* caller save register*/
    genCallerSaveReg(count);
    for(i = 0; i < PARAREGNUM && i < count; i++){
        p = isInPool(c->paraList[i]);
        if(p == -1){
            loadIdent(reg_a0 + NOTPARAREGNUM + i, c->paraList[i]);
        }else{
            genCal_r(OR, reg_a0 + NOTPARAREGNUM + i, p, reg_zero);
        }
    }
    clearPool();
    /* allocate memery for parameters */
    if(count > 0) genAllocate(count);
    genCal_r(OR, reg_fp, reg_sp, reg_zero);
    genJump(JAL, 0, a->name);
    if(count > 0) genFree(count);
    /* the fp of this frame need to pop */
    genLd(LW, reg_fp, reg_sp, frame_FP);
    /* pop caller save register */
    genCallerPopReg(count);
    if(a->type != voids){
        rd = isInPool(res);
        if(rd == -1){
            rd = allocReg(res);
            mapReg(res, rd);
        }
        genCal_r(OR, rd, reg_v1, reg_zero);
        res->dirty = 1;
    }
}

void genPrintf(struct FCode *c){
    struct SymItem *a;
    struct SymItem *b;
    struct SymItem *res;
    struct SymItem *pa;
    int op;
    int count;
    int i;
    int offset;
    int p;

    op = c->op;
    a = c->a;
    b = c->b;
    res = c->res;
    count = c->count;

    pa = 0;
    /* save a0 */
    if(regPool[reg_a0] != 0 && regPool[reg_a0]->kind != constant){
        storeIdent(reg_a0, regPool[reg_a0]);
        pa = regPool[reg_a0];
        regPool[reg_a0] = 0;
    }
    /* load a0 */
    for(i = 0; i < count; i++){
        switch(c->paraList[i]->type){
            case ints:{
                genCal_i(LI, reg_v0, 0, 1);
                p = isInPool(c->paraList[i]);
                if(p == -1){
                    loadIdent(reg_a0, c->paraList[i]);
                }else{
                    genCal_r(OR, reg_a0, p, reg_zero);
                }
                break; 
            }
            case chars:{
                genCal_i(LI, reg_v0, 0, 11);
                //loadIdent(reg_a0, c->paraList[i]);
                p = isInPool(c->paraList[i]);
                if(p == -1){
                    loadIdent(reg_a0, c->paraList[i]);
                }else{
                    genCal_r(OR, reg_a0, p, reg_zero);
                }
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
    /* pop a0 */
    if(pa != 0){
        loadIdent(reg_a0, pa);
        regPool[reg_a0] = pa;
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
    int rd;

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
        //storeIdent(reg_v0, c->paraList[i]);
        rd = isInPool(c->paraList[i]);
        if(rd == -1){
            rd = allocReg(c->paraList[i]);
            mapReg(c->paraList[i], rd);
        }
        genCal_r(OR, rd, reg_v0, reg_zero);
        c->paraList[i]->dirty = 1;
    }
}

void paraRegMap(){
    int i;
    int cp;

    for(i = 0; i < tab_p; i++){
        if(tab[i] == func) break;
    }

    if(i == tab_p){
        printf("paraRegMap error\n");
        return;
    }

    cp = i + 1;

    for(i = 0; i < func->paras && i < PARAREGNUM; i++){
        mapReg(tab[cp + i], reg_a0 + NOTPARAREGNUM + i);
    }
}

void genMips(){
    int index;
    struct FCode *c;
    char funcExit[IDLMX] = {0};


    index = 0;
    gloBlock = btab[0];
    /* data segment */
    genDataSeg();
    
    //genAllocate(0x40000);

    /* gen jal main */
    genProgStart();
    
    /* generate mips code for function */
    for(index = 1; index < btab_p; index++){
        curBB = -1;
        locBlock = btab[index];
        func = tab[locBlock->cur];
        genLiveIntervals(func);
        cd = func->ref;
        /* gen label for function */
        genLabel(mCode[cd]);
        /* allocate register and a0-a3 */
        paraRegMap();
        /* save frame */
        /* allocate local memery */
        genPrologue(func);
        if(GLOBALREGALLOCFLAG) allocGlobalReg();
        if(COUNTASSIGNREG) allocRegWithCount();
        genCalleeSaveFrame();
        for(cd = func->ref + 1; cd < func->end; cd++){
            /* check whether a new base block */
            c = mCode[cd];
            if(curBB < c->belong2BB){
                if(GLOBALREGALLOCFLAG && curBB != -1) clearGlobalReg(curBB);
                curBB = c->belong2BB;
                if(GLOBALREGALLOCFLAG) loadGlobalReg(curBB);
                clearPool();
            }
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
        sprintf(funcExit, "%s_end", func->name);
        clearPool();
        if(MIPSCONSOLE) printf("label_%s:\n", funcExit);
        if(isGenAsm) fprintf(asmfp, "label_%s:\n", funcExit);
        genCalleePopFrame();
        genEpilogue();
        genJump(JR, reg_ra, 0);
    }
}