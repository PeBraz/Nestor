#include <stdio.h>
#include "nestor.h"


int test_immediate_lda() {

    struct nestor Nes;

    Nes.regs.pc = 0x0601;
    nestor_set_byte(&Nes, 0x0601, 0x22);

    immediate(&Nes, lda);
    
    if (Nes.regs.acc != 0x22)   return -1;
    if (Nes.regs.status & ZERO_FLAG != ZERO_FLAG)   return -1;
    if (Nes.regs.status & NEGATIVE_FLAG != NEGATIVE_FLAG) return -1;

    return 0;

}


int test_absolute_sta() {

    struct nestor nes;

    nes.regs.acc = 45;
    absolute(&nes, sta, 0x3F43); // store 45 in memory position
    
    if (nes.memory[0x3F43] != 45)   return -1;
    return 0;

}

int test_adc() {

    struct nestor nes;
    nes = (struct nestor){.regs={0}};

    immediate(&nes, adc, 45);   //add 45 to acc

    if (nes.regs.acc != 45) return -1;
    if (nes.regs.status & ZERO_FLAG) return -1;
    if (nes.regs.status & CARRY_FLAG) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return -1; //failing here
    if (nes.regs.status & OVERFLOW_FLAG) return -1;
    if (nes.regs.status & DECIMAL_FLAG) return -1;

    return 0;
}

int test_adc_zero()
{
    struct nestor nes;
    nes = (struct nestor){.regs={0}};
    immediate(&nes, adc, 0);    

    if (!(nes.regs.status & ZERO_FLAG)) return -1;

    if (nes.regs.acc != 0) return -1;
    if (nes.regs.status & CARRY_FLAG) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return -1;
    if (nes.regs.status & OVERFLOW_FLAG) return -1;

    return 0;
}

int test_adc_negative()
{
    struct nestor nes;
    //negative
    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 128;     // -1 (1000)
    immediate(&nes, adc, 1);    // -2 (1001)

    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;

    if (nes.regs.acc != 129) return -1;
    if (nes.regs.status & CARRY_FLAG) return -1;
    if (nes.regs.status & ZERO_FLAG) return -1;
    if (nes.regs.status & OVERFLOW_FLAG) return -1;

    return 0;
}

int test_adc_overflow()
{
    struct nestor nes;
    //over +127 overflow
    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 127;     
    immediate(&nes, adc, 1);    // 128 -> -1 (1000)

    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
    if (!(nes.regs.status & OVERFLOW_FLAG)) return -1;

    if (nes.regs.acc != 128) return -1;
    if (nes.regs.status & CARRY_FLAG) return -1;
    if (nes.regs.status & ZERO_FLAG) return -1;

    return 0;
}

int test_adc_carry()
{
    struct nestor nes;
    // carry 
    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 255; //-128
    immediate(&nes, adc, 1); // 0

    if (!(nes.regs.status & CARRY_FLAG)) return -1;
    if (!(nes.regs.status & ZERO_FLAG)) return -1;

    if (nes.regs.acc != 0) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return -1;
    if (nes.regs.status & OVERFLOW_FLAG) return -1;

    return 0;
}

int test_and()
{
    struct nestor nes;

    nes = (struct nestor){.regs={0}};
    nes.regs.acc = 0;   //-128
    immediate(&nes, and, 0); // 0

    if (!(nes.regs.status & ZERO_FLAG)) return -1;
    if (nes.regs.acc != 0) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return -1;

    nes.regs.acc = 0x80;
    immediate(&nes, and, 0x80);

    if (nes.regs.status & ZERO_FLAG) return -1;
    if (nes.regs.acc != 0x80) return -1;
    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;


    return 0;

}

int test_asl()
{
    struct nestor nes = (struct nestor){.regs={0}};

    nes.regs.acc = 0xFF;
    accumulator(&nes, asl);

    if (nes.regs.acc != 0xFE) return -1;
    if (!(nes.regs.status & CARRY_FLAG)) return -1;
    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
    if (nes.regs.status & ZERO_FLAG) return -1;

    nes.regs.acc = 0;
    accumulator(&nes, asl);

    if (!(nes.regs.status & ZERO_FLAG)) return -1;
    if (nes.regs.acc != 0) return -1;
    if (nes.regs.status & NEGATIVE_FLAG) return -1;
    if (nes.regs.status & CARRY_FLAG) return -1;

    return 0;
}

int test_bit()
{
    struct nestor nes;

    nes.regs.acc = 0x0F;
    nes.memory[0x1111] = 0xF0;  // will set bit 7 and 6
    absolute(&nes, bit, 0x1111);

    if (!(nes.regs.status & NEGATIVE_FLAG)) return -1;
    if (!(nes.regs.status & OVERFLOW_FLAG)) return -1;
    if (!(nes.regs.status & ZERO_FLAG)) return -1;
    
    return 0;
}


int main()
{
    printf( "1 %s\n", test_immediate_lda() == 0? "SUCCESS" :"FAILURE");
    printf( "2 %s\n",  test_absolute_sta() == 0? "SUCCESS" :"FAILURE");
    printf( "3 %s\n",  test_adc() == 0? "SUCCESS" :"FAILURE");
    printf( "4 %s\n",  test_adc_zero() == 0? "SUCCESS" :"FAILURE");
    printf( "5 %s\n",  test_adc_negative() == 0? "SUCCESS" :"FAILURE");
    printf( "6 %s\n",  test_adc_overflow() == 0? "SUCCESS" :"FAILURE");
    printf( "7 %s\n",  test_adc_carry() == 0? "SUCCESS" :"FAILURE");
    printf( "8 %s\n",  test_and() == 0? "SUCCESS" :"FAILURE");
    printf( "9 %s\n",  test_asl() == 0? "SUCCESS" :"FAILURE");
    printf( "10 %s\n",  test_bit() == 0? "SUCCESS" :"FAILURE");

    return 0;
}