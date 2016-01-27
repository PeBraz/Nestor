#include <stdio.h>
#include "nestor.h"


#define ZERO_SHOULD_NOT_BE_SET 1
#define ZERO_SHOULD_BE_SET 2
#define CARRY_SHOULD_NOT_BE_SET 3
#define CARRY_SHOULD_BE_SET 4
#define SIGN_SHOULD_NOT_BE_SET 5
#define SIGN_SHOULD_BE_SET 6
#define INTERRUPT_SHOULD_NOT_BE_SET 7
#define INTERRUPT_SHOULD_BE_SET 8
#define OVERFLOW_SHOULD_NOT_BE_SET 9
#define OVERFLOW_SHOULD_BE_SET 10
#define DECIMAL_FLAG_SHOULD_NOT_BE_SET 11
#define DECIMAL_FLAG_SHOULD_BE_SET 12
#define WRONG_VALUE 13

#define TEST(test) \
    int e_##test = test();\
    printf(#test ": %s (%d)\n", (e_##test!=0)? "failed": "success", e_##test);

int test_immediate() {

    struct nestor Nes;

    Nes.regs.pc = 0x0600;
    Nes.memory[0x0601] = 0x22;  
    immediate(&Nes, lda);
    
    if (Nes.regs.acc != 0x22)   return WRONG_VALUE;
    if (Nes.regs.status & ZERO_FLAG)   return ZERO_SHOULD_NOT_BE_SET;
    if (Nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;

    return 0;

}

int test_absolute() 
{
    struct nestor nes;

    nes.regs.acc = 45;
    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0x34;
    nes.memory[0x0602] = 0x12;
    absolute(&nes, sta); // store 45 in memory position
    
    if (nes.memory[0x1234] != 45)   return WRONG_VALUE;
    return 0;

}


int test_adc() {

    struct nestor nes;
    nes = (struct nestor){.regs={0}};

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 45;
    immediate(&nes, adc);   

    if (nes.regs.acc != 45) return WRONG_VALUE;
    if (nes.regs.status & ZERO_FLAG) return ZERO_SHOULD_NOT_BE_SET;
    if (nes.regs.status & CARRY_FLAG) return CARRY_SHOULD_NOT_BE_SET;
    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET; 
    if (nes.regs.status & OVERFLOW_FLAG) return OVERFLOW_SHOULD_NOT_BE_SET;
    if (nes.regs.status & DECIMAL_FLAG) return DECIMAL_FLAG_SHOULD_NOT_BE_SET;

    return 0;
}

int test_adc_zero()
{
    struct nestor nes;
    nes = (struct nestor){.regs={0}};

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0;
    immediate(&nes, adc);    

    if (!(nes.regs.status & ZERO_FLAG)) return ZERO_SHOULD_BE_SET;

    if (nes.regs.acc != 0) return WRONG_VALUE;
    if (nes.regs.status & CARRY_FLAG) return CARRY_SHOULD_NOT_BE_SET;
    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;
    if (nes.regs.status & OVERFLOW_FLAG) return OVERFLOW_SHOULD_NOT_BE_SET;

    return 0;
}

int test_adc_negative()
{
    struct nestor nes;
    //negative
    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 128;     // -1 (1000)

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 1; // -2 (1001)
    immediate(&nes, adc);    

    if (!(nes.regs.status & NEGATIVE_FLAG)) return SIGN_SHOULD_BE_SET;

    if (nes.regs.acc != 129) return WRONG_VALUE;
    if (nes.regs.status & CARRY_FLAG) return CARRY_SHOULD_NOT_BE_SET;
    if (nes.regs.status & ZERO_FLAG) return ZERO_SHOULD_NOT_BE_SET;
    if (nes.regs.status & OVERFLOW_FLAG) return OVERFLOW_SHOULD_NOT_BE_SET;

    return 0;
}

int test_adc_overflow()
{
    struct nestor nes;
    //over +127 overflow
    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 127;   

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 1;   

    immediate(&nes, adc);    // 128 -> -1 (1000)

    if (!(nes.regs.status & NEGATIVE_FLAG)) return SIGN_SHOULD_BE_SET;
    if (!(nes.regs.status & OVERFLOW_FLAG)) return OVERFLOW_SHOULD_BE_SET;

    if (nes.regs.acc != 128) return WRONG_VALUE;
    if (nes.regs.status & CARRY_FLAG) return CARRY_SHOULD_NOT_BE_SET;
    if (nes.regs.status & ZERO_FLAG) return ZERO_SHOULD_NOT_BE_SET;

    return 0;
}

int test_adc_carry()
{
    struct nestor nes;
    // carry 
    nes = (struct nestor){.regs={0}};

    nes.regs.acc = 255; //-128
    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 1;  

    immediate(&nes, adc); // 0

    if (!(nes.regs.status & CARRY_FLAG)) return CARRY_SHOULD_BE_SET;
    if (!(nes.regs.status & ZERO_FLAG)) return ZERO_SHOULD_BE_SET;

    if (nes.regs.acc != 0) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;
    if (nes.regs.status & OVERFLOW_FLAG) return OVERFLOW_SHOULD_NOT_BE_SET;

    return 0;
}

int test_and_1()
{
    struct nestor nes;

    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 0;   //-128
    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0;  

    immediate(&nes, and); // 0

    if (!(nes.regs.status & ZERO_FLAG)) return ZERO_SHOULD_BE_SET;
    if (nes.regs.acc != 0) return WRONG_VALUE;
    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;
    
    return 0;
}

int test_and_2()
{
    struct nestor nes;
    nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0x80;
    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0xFF;

    immediate(&nes, and);

    if (nes.regs.status & ZERO_FLAG) return ZERO_SHOULD_NOT_BE_SET;
    if (nes.regs.acc != 0x80) return WRONG_VALUE;
    if (!(nes.regs.status & NEGATIVE_FLAG)) return SIGN_SHOULD_BE_SET;


    return 0;

}

int test_asl_1()
{
    struct nestor nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0xFF;
    accumulator(&nes, asl);

    if (nes.regs.acc != 0xFE) return WRONG_VALUE;
    if (!(nes.regs.status & CARRY_FLAG)) return ;
    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
    if (nes.regs.status & ZERO_FLAG) return -1;
    
    return 0;
}

int test_asl_2()
{
    struct nestor nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0;
    accumulator(&nes, asl);

    if (!(nes.regs.status & ZERO_FLAG)) return ZERO_SHOULD_BE_SET;
    if (nes.regs.acc != 0) return WRONG_VALUE;
    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;
    if (nes.regs.status & CARRY_FLAG) return CARRY_SHOULD_NOT_BE_SET;

    return 0;
}

// A & M == 0 -> set Z = 1
int test_bit_1()
{
    struct nestor nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0x0F;

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0x34; // points to memory piece to apply bit op
    nes.memory[0x0602] = 0x12;
    nes.memory[0x1234] = 0xF0;  // will set bit 7 and 6

    absolute(&nes, bit);

    if (!(nes.regs.status & NEGATIVE_FLAG)) return SIGN_SHOULD_BE_SET;
    if (!(nes.regs.status & OVERFLOW_FLAG)) return OVERFLOW_SHOULD_BE_SET;
    if (!(nes.regs.status & ZERO_FLAG)) return ZERO_SHOULD_BE_SET;
    
    return 0;
}
// A & M != 0 -> set Z = 0
int test_bit_2()
{
    struct nestor nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0x01;

    nes.regs.pc = 0x0600;
    nes.memory[0x0601] = 0x34; // points to memory piece to apply bit op
    nes.memory[0x0602] = 0x12;
    nes.memory[0x1234] = 0x01;  // will set bit 7 and 6

    absolute(&nes, bit);

    if (nes.regs.status & NEGATIVE_FLAG) return SIGN_SHOULD_NOT_BE_SET;
    if (nes.regs.status & OVERFLOW_FLAG) return OVERFLOW_SHOULD_NOT_BE_SET;
    if (nes.regs.status & ZERO_FLAG) return ZERO_SHOULD_NOT_BE_SET;
    
    return 0;
}


int main()
{
    TEST(test_immediate);
    TEST(test_absolute);
    TEST(test_adc);
    TEST(test_adc_zero);
    TEST(test_adc_negative);
    TEST(test_adc_overflow);
    TEST(test_adc_carry);
    TEST(test_and_1);
    TEST(test_and_2);
    TEST(test_asl_1);
    TEST(test_asl_2);
    TEST(test_bit_1);
    TEST(test_bit_2);


    return 0;
}