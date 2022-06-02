#include <stdint.h>
#define u16 uint16_t

/* Register */
enum Register
{
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, /* program counter , store which address we are now */
    R_COND,
    R_COUNT
};

/* condition flag use in condition register */
enum
{
    FL_POS = 1 << 0, /* P -> 1 */
    FL_ZRO = 1 << 1, /* Z -> 2 */
    FL_NEG = 1 << 2, /* N -> 4 */
};
