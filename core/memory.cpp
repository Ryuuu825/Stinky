#include "memory.h"


u16 mem_read(u16 address) 
{
    extern u16 mem[UINT16_MAX];
    return mem[address];
};

void mem_write(u16 address , u16 data) 
{
    extern u16 mem[UINT16_MAX];
    mem[address] = data;
};
