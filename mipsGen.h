#ifndef _MIPSGEN_H
#define _MIPSGEN_H

#define IMM_MIN -32768
#define IMM_MAX 32767
#define REGPOOLSIZE 32
#define tPOOLSIZE 10
#define TMPSTACK 1000
#define NOTPARAREGNUM 1
#define PARAREGNUM (4 - NOTPARAREGNUM)
#define frame_RA 0
#define frame_FP 1
#define frame_S0 2
#define frame_S1 3
#define frame_S2 4
#define frame_S3 5
#define frame_S4 6
#define frame_S5 7
#define frame_S6 8
#define frame_S7 9

enum mipsReg{
    reg_zero, reg_at, 
    reg_v0, reg_v1,
    reg_a0, reg_a1, reg_a2, reg_a3,
    reg_t0, reg_t1, reg_t2, reg_t3, reg_t4, reg_t5, reg_t6, reg_t7, reg_t8, reg_t9,
    reg_s0, reg_s1, reg_s2, reg_s3, reg_s4, reg_s5, reg_s6, reg_s7,
    reg_k0, reg_k1,
    reg_gp, reg_sp, reg_fp, reg_ra
};

enum mipsIns{
    ADD, ADDI, ADDIU, ADDU, SUB, SUBU,
    AND, ANDI, NOR, OR, ORI, XOR, XORI,
    SLL, SLLV, SRA, SRAV, SRL, SRLV,
    DIV, DIVU, MUL, MULT, MULTU, MFHI, MFLO, 
    SLT, SLTI, SLTIU, SLTU,
    J, JAL, JALR, JR,
    BEQ, BGEZ, BGTZ, BLEZ, BLTZ, BNE,
    LB, LBU, LH, LHU, LUI, LW,
    SB, SH, SW,
    SYSCALL, NOP, 
    LI
};

struct tVarPool{
    int index[10];
    int adr[10];
};

void genMips();

#endif