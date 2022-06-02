#include <stdio.h>

#include "vm.h"

/* 
 *  add {0001}
 *  Take two number, add them add and store it into a register
 *  0001 000 000 000000
 *       
 *  The first four bits (i.e. 0001) is the opcode for ADD.
 *  The next three bits are destination register, which store the result.
 *  The next three bits are source register, containing the first number to add.
 *  
 *  There are two mode to operate
 *  1) 0001 000 000 000 000
 *  2) 0001 000 000 1 00000
 *  
 *  The mode to identfy by the bit in index 5.
 *  In first mode the value going to add value are stored in second register.
 *  In secode mode the value going to add embedded into the instruction. Max value is 32 (2^5).
 *  
 *  
 *    
 */
void add(u16 instruction)
{
    extern u16 reg[16];
    u16 destination = (instruction >> 9) & 0b111;
    u16 source = (instruction >> 6) & 0b111;
    u16 mode = (instruction >> 5) & 0b1;

    if (mode) // the second mode
    {
        u16 value = sign_extend(instruction & 0x1F, 5);
        reg[destination] = reg[source] + value;
    }
    else // the first mode
    {
        u16 source2 = instruction & 0b0000000000000111;
        reg[destination] = reg[source] + reg[source2];
    }   

    update_flag(destination);

#ifdef Debug
    // TODO: more information is needed I think
    printf("[\033[1;31mADD\033[0m] Instruction: 0x%hu | Output: %hu\n",
            instruction , reg[destination]);

    
#endif

}


/* 
 *  ldi -- load indirect {1010}
 *  1010 000 000000000
 *  The first four bits is the opcode for ldi
 *  The next three bits is for destination register
 *  The next nine bits for program counter offest 
 *  
 *  This instruction load the the memory address in the 
 * 
 */
void ldi(u16 instruction)
{
    extern u16 reg[16];
    u16 destination = (instruction >> 9) & 0b111;
    u16 pc_offset = sign_extend(instruction & 0x1FF , 9);
    reg[destination] = mem_read(mem_read(reg[R_PC] + pc_offset));

    update_flag(destination);

#ifdef Debug
    printf("[\033[1;31mLDI\033[0m] Instruction: 0x%hu | Dest: %hu | Sour: %hu | Value: %hu\n",
              instruction , destination , (u16)(reg[R_PC] + pc_offset) , mem_read(reg[R_PC] + pc_offset));
#endif
}


void op_and(u16 instruction) 
{
    extern u16 reg[16];
    u16 destination = (instruction >> 9) & 0b111;
    u16 source = (instruction >> 6) & 0b111;    
    u16 mode = (instruction >> 5) & 0b1;

    if (mode) // imm5 
    {
        u16 target = sign_extend(instruction & 0x1F , 5);
        reg[destination] = reg[source] & target;
    }
    else 
    {
        u16 source2 = instruction & 0b111;
        reg[destination] = reg[source] & reg[source2];
    }

    update_flag(destination);

#ifdef Debug
    if(mode) // imm5 
    {
        printf("[\033[1;31mAND\033[0m] Instruction: 0x%hu | Source1 (reg[%hu]): %hu | Imm5: %hu | Result(reg[%hu]): %hu\n",
              instruction , source ,  reg[source] , (sign_extend(instruction & 0b11111 , 5)) , destination, reg[destination]);
    }
    else
    {
        printf("[\033[1;31mAND\033[0m] Instruction: 0x%hu | Source(reg[%hu]): %hu | Source2(reg[%hu]): %hu | Result(reg[%hu]): %hu\n",
              instruction , source , reg[source] , (u16) (instruction & 0b111)  , reg[(instruction & 0b111) ], destination, reg[destination]);
    }
    
#endif
}


void op_not(u16 instruction)
{
    extern u16 reg[16];
    u16 destination = (instruction >> 9) & 0b111;
    u16 source = (instruction >> 6) & 0b111;

    reg[destination] = ~(reg[source]);

    update_flag(reg[destination]);

#ifdef Debug
    printf("[\033[1;31mNOT\033[0m] Instruction: 0x%hu | Source(reg[%hu]): %hu | Result(reg[%hu]): %hu\n",
              instruction , source ,  reg[source] , destination, reg[destination]);

#endif

}

/**
 * br -- branch 
 * 0000 111 000000000
 * First four bits are opcode 
 * next three bits are for condition flag
 * next nine bits are for program counter
 */
void branch(u16 instrcution) 
{
    extern u16 reg[16];
    
    u16 pc_offset = sign_extend(instrcution & 0x1FF , 9);
    u16 flag = (instrcution >> 9)  & 0b111;

    if (flag & reg[R_COND])
    {
        reg[R_COND] += pc_offset;
    };
}

/**
 * JMP -- jump
 * 1100 000 000 000000
 * First four bits are opcode 1100
 * next three bits are non use
 * next three bits are the destination register
 */
void jump(u16 instruction)
{
    extern u16 reg[16];

    u16 loc = (instruction >> 6) & 0b111;
    
    reg[R_COUNT] = reg[loc];

#ifdef Debug
    printf("[\033[1;31mJMP\033[0m] Instruction: 0x%hu | Jmmp to %hu\n",
              instruction , reg[R_COUNT]);
#endif
}

/**
 *  JSR / JSRR
 *  0100 
 *  Frist four bits are opcode 0100
 * 
 *  This opertaion have two mode 
 *  1) 0100 1 00000000000 (JSR)
 *  remaining 11 bits used for offset the program counter
 * 
 *  2) 0100 0 00 000 000000 (JSRR)
 *    index 9 - 11 used for tell which what register store the memory adress
 */
void jsr(u16 instruction)
{
    extern u16 reg[16];

    reg[R_R7] = reg[R_COUNT];

    u16 flag = (instruction >> 11 ) & 0b1;

    if (flag) 
    {
        u16 offset = sign_extend(instruction & 0x7FF , 11);
        reg[R_COUNT] += offset;
    }
    else 
    {
        u16 loc = (instruction >> 6) &0b111;
        reg[R_COUNT] = reg[loc];
    }

}


void load(u16 instruction)
{
    extern u16 reg[16];
    u16 destination = (instruction >> 9) & 0b111;
    u16 offset = sign_extend(instruction & 0x1FF , 9);

    reg[destination] = mem_read(reg[R_COUNT] + offset);
    update_flag(destination);
}



uint16_t sign_extend(uint16_t x, int bit_count)
{
    if ((x >> (bit_count - 1)) & 1) {
        x |= (0xFFFF << bit_count);
    }
    return x;
}

void update_flag(u16 index) 
{
    extern u16 reg[16];
    if (reg[index] == 0) 
    {
        reg[R_COND] = FL_ZRO;
    }
    else if (reg[index] >> 15) // the left-most bit is 1
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}