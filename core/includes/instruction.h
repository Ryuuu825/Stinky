#pragma once 
#include <stdint.h>
#include "vmdef.h"
/* instruction set */
enum
{
    OP_BR = 0, /* branch */
    OP_ADD,    /* add  */
    OP_LD,     /* load */
    OP_ST,     /* store */
    OP_JSR,    /* jump register */
    OP_AND,    /* bitwise and */
    OP_LDR,    /* load register */
    OP_STR,    /* store register */
    OP_RTI,    /* unused */
    OP_NOT,    /* bitwise not */
    OP_LDI,    /* load indirect */
    OP_STI,    /* store indirect */
    OP_JMP,    /* jump */
    OP_RES,    /* reserved (unused) */
    OP_LEA,    /* load effective address */
    OP_TRAP    /* execute trap */
};


uint16_t sign_extend(uint16_t x, int bit_count);
void update_flag(u16 reg);

void add(u16 instruction);
void ldi(u16 instrution);
void op_and(u16 instruction);
void op_not(u16 instruction);
void branch(u16 instrcution);
void jump(u16 instruction);
void jsr(u16 instruction);
void load(u16 instruction);
void ldi(u16 instruction);