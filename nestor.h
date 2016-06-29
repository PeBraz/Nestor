#ifndef NESTOR_H
#define NESTOR_H


#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "graphi.h"

#define DEBUG


#define NES_MEM_SIZE 65536

#define IS_NEGATIVE 0x80
#define SIGNED_BIT 0x80

//Status Flags 
#define NEGATIVE_FLAG 0x80//or sign flag
#define OVERFLOW_FLAG 0x40
#define BREAK_FLAG 0x10
#define DECIMAL_FLAG 0x08
#define INTERRUPT_FLAG 0x04
#define ZERO_FLAG 0x02
#define CARRY_FLAG 0x01


//Operation codes
#define BRK 0x00
#define ORA_INDIRECT_X 0x01
#define ORA_ZERO_PAGE 0x05
#define ASL_ZERO_PAGE 0x06
#define PHP 0x08
#define ORA_IMMEDIATE 0x09
#define ASL_ACCUMULATOR 0x0A
#define ORA_ABSOLUTE 0x0D
#define ASL_ABSOLUTE 0x0E
#define BPL 0x10
#define ORA_INDIRECT_Y 0x11
#define ORA_ZERO_PAGE_X 0x15 
#define ASL_ZERO_PAGE_X 0x16
#define CLC 0x18
#define ORA_ABSOLUTE_Y 0x19
#define ORA_ABSOLUTE_X 0x1D
#define ASL_ABSOLUTE_X 0x1E
#define JSR 0x20
#define AND_INDIRECT_X 0x21
#define BIT_ZERO_PAGE 0x24
#define AND_ZERO_PAGE 0x25
#define ROL_ZERO_PAGE 0x26
#define PLP 0x28
#define AND_IMMEDIATE 0x29
#define ROL_ACCUMULATOR 0x2A
#define BIT_ABSOLUTE 0x2C
#define AND_ABSOLUTE 0x2D
#define ROL_ABSOLUTE 0x2E
#define BMI 0x30
#define AND_INDIRECT_Y 0x31
#define AND_ZERO_PAGE_X 0x35
#define ROL_ZERO_PAGE_X 0x36
#define SEC 0x38
#define AND_ABSOLUTE_Y 0x39
#define AND_ABSOLUTE_X 0x3D
#define ROL_ABSOLUTE_X 0x3E
#define RTI 0x40
#define EOR_INDIRECT_X 0x41
#define EOR_ZERO_PAGE 0x45
#define LSR_ZERO_PAGE 0x46
#define PHA 0x48
#define EOR_IMMEDIATE 0x49
#define LSR_ACCUMULATOR 0x4A
#define JMP_ABSOLUTE 0x4C
#define EOR_ABSOLUTE 0x4D
#define LSR_ABSOLUTE 0x4E
#define BVC 0x50
#define EOR_INDIRECT_Y 0x51
#define EOR_ZERO_PAGE_X 0x55
#define LSR_ZERO_PAGE_X 0x56
#define CLI 0x58
#define EOR_ABSOLUTE_Y 0x59
#define EOR_ABSOLUTE_X 0x5D //confirm
#define LSR_ABSOLUTE_X 0x5E
#define RTS 0x60
#define ADC_INDIRECT_X 0x61
#define ADC_ZERO_PAGE 0x65
#define ROR_ZERO_PAGE 0x66
#define PLA 0x68
#define ADC_IMMEDIATE 0x69 
#define ROR_ACCUMULATOR 0x6A
#define JMP_INDIRECT 0x6C
#define ADC_ABSOLUTE 0x6D
#define ROR_ABSOLUTE 0x6E
#define BVS 0x70
#define ADC_INDIRECT_Y 0x71
#define ADC_ZERO_PAGE_X 0x75
#define ROR_ZERO_PAGE_X 0x76
#define SEI 0x78
#define ADC_ABSOLUTE_Y 0x79
#define ADC_ABSOLUTE_X 0x7D
#define ROR_ABSOLUTE_X 0x7E
#define STA_INDIRECT_X 0x81
#define STY_ZERO_PAGE 0x84 
#define STA_ZERO_PAGE 0x85
#define STX_ZERO_PAGE 0x86
#define DEY 0x88
#define TXA 0x8A
#define STY_ABSOLUTE 0x8C
#define STA_ABSOLUTE 0x8D
#define STX_ABSOLUTE 0x8E
#define BCC 0x90 
#define STA_INDIRECT_Y 0x91
#define STY_ZERO_PAGE_X 0x94
#define STA_ZERO_PAGE_X 0x95
#define STX_ZERO_PAGE_Y 0x96
#define TYA 0x98
#define STA_ABSOLUTE_Y 0x99
#define TXS 0x9A
#define STA_ABSOLUTE_X 0x90
#define LDY_IMMEDIATE 0xA0
#define LDA_INDIRECT_X 0xA1
#define LDX_IMMEDIATE 0xA2
#define LDY_ZERO_PAGE 0xA4 
#define LDA_ZERO_PAGE 0xA5
#define LDX_ZERO_PAGE 0xA6
#define TAY 0xA8
#define LDA_IMMEDIATE 0xA9
#define TAX 0xAA
#define LDY_ABSOLUTE 0xAC
#define LDA_ABSOLUTE 0xAD  
#define LDX_ABSOLUTE 0xAE
#define BCS 0xB0
#define LDA_INDIRECT_Y 0xB1
#define LDY_ZERO_PAGE_X 0xB4 
#define LDA_ZERO_PAGE_x 0xB5
#define LDX_ZERO_PAGE_Y 0xB6
#define CLV 0xB8
#define LDA_ABSOLUTE_Y 0xB9 
#define TSX 0xBA
#define LDY_ABSOLUTE_X 0xBC 
#define LDA_ABSOLUTE_X 0xBD
#define LDX_ABSOLUTE_Y 0xBE
#define CPY_IMMEDIATE 0xC0
#define CMP_INDIRECT_X 0xC1
#define CPY_ZERO_PAGE 0xC4
#define CMP_ZERO_PAGE 0xC5
#define DEC_ZERO_PAGE 0xC6
#define INY 0xC8
#define CMP_IMMEDIATE 0xC9
#define DEX 0xCA
#define CPY_ABSOLUTE 0xCC
#define CMP_ABSOLUTE 0xCD
#define DEC_ABSOLUTE 0xCE
#define BNE 0xD0
#define CMP_INDIRECT_Y 0xD1
#define CMP_ZERO_PAGE_X 0xD5 
#define DEC_ZERO_PAGE_x 0xD6
#define CLD 0xD8
#define CMP_ABSOLUTE_Y 0xD9
#define CMP_ABSOLUTE_X 0xDD
#define DEC_ABSOLUTE_X 0xDE
#define CPX_IMMEDIATE 0xE0
#define SBC_INDIRECT_X 0xE1
#define CPX_ZERO_PAGE 0xE4
#define SBC_ZERO_PAGE 0xE5
#define INC_ZERO_PAGE 0xE6
#define INX 0x08
#define SBC_IMMEDIATE 0xE9
#define NOP 0xEA 
#define CPX_ABSOLUTE 0xEC
#define SBC_ABSOLUTE 0xED
#define INC_ABSOLUTE 0xEE
#define BEQ 0xF0 
#define SBC_INDIRECT_Y 0xF1
#define SBC_ZERO_PAGE_X 0xF5
#define INC_ZERO_PAGE_X 0xF6
#define SED 0xF8
#define SBC_ABSOLUTE_Y 0xF9
#define SBC_ABSOLUTE_X 0xFD
#define INC_ABSOLUTE_X 0xFE

struct nestor {
    void (*opcodes[0xFF])(struct nestor *);
    uint8_t memory[NES_MEM_SIZE];

    struct graphics video;
    struct {
        uint8_t acc;
        uint8_t x;
        uint8_t y;
        uint8_t status;
        uint16_t pc;
        uint16_t sp;
    } regs;
};

#ifdef DEBUG 

#define DBG(msg) fprintf(stderr, "Debug [%s:%d]: " # msg, __FILE__, __LINE__); 
#define DBGF(msg,...) fprintf(stderr, "Debug [%s:%d]: " # msg, __FILE__, __LINE__, __VA_ARGS__);
#define NES_DEF(OP, MODE) \
    void nes_call_ ## OP ## _ ## MODE (struct nestor * nes) {\
                    printf("DEBUG: %s - %s\n", #OP, #MODE);\
                    MODE(nes, OP);\
                }
#else
#define DBG(msg, ...)
#define NES_DEF(OP, MODE) \
    void nes_call_ ## OP ## _ ## MODE (struct nestor * nes) {\
                    MODE(nes, OP);\
                }
#endif


struct nestor nestor_init();
int nestor_cartridge(struct nestor *, char *);


void nestor_st_push(struct nestor *, uint8_t);
uint8_t nestor_st_pop(struct nestor *);

//Use it to take a byte from memory (little endian)
uint8_t * nestor_load(struct nestor *, uint16_t);

void implied(struct nestor *, void(*operation)(struct nestor *));
void immediate(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void absolute(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void jmp_absolute(struct nestor *, void(*operation)(struct nestor *,uint16_t));
void accumulator(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void zero_page(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void absolute_x(struct nestor *,void(*operation)(struct nestor *,uint8_t*));
void absolute_y(struct nestor *,void(*operation)(struct nestor *,uint8_t*));
void zero_page_x(struct nestor *,void(*operation)(struct nestor *,uint8_t*));
void zero_page_y(struct nestor *,void(*operation)(struct nestor *,uint8_t*));
void indirect(struct nestor *, void(*operation)(struct nestor *,uint16_t));
void indirect_x(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void indirect_y(struct nestor *, void(*operation)(struct nestor *,uint8_t*));
void relative(struct nestor *, void(*operation)(struct nestor *,uint8_t));



void adc(struct nestor *, uint8_t*);
void and(struct nestor *, uint8_t*);
void asl(struct nestor *, uint8_t*);
void bcc(struct nestor *, uint8_t);
void bcs(struct nestor *, uint8_t);
void beq(struct nestor *, uint8_t);
void bne(struct nestor *, uint8_t);
void bit(struct nestor *, uint8_t*);
void bmi(struct nestor *, uint8_t);
void bpl(struct nestor *, uint8_t);
void bvc(struct nestor *, uint8_t);
void bvs(struct nestor *, uint8_t);
void brk(struct nestor *);
void clc(struct nestor *);
void cld(struct nestor *);
void cli(struct nestor *);
void clv(struct nestor *);
void cmp(struct nestor *, uint8_t*);
void cpx(struct nestor *, uint8_t*);
void cpy(struct nestor *, uint8_t*);
void dec(struct nestor *, uint8_t*);
void dex(struct nestor *);
void dey(struct nestor *);
void eor(struct nestor *, uint8_t*);
void inc(struct nestor *, uint8_t*);
void inx(struct nestor *);
void iny(struct nestor *);
void jmp(struct nestor *, uint16_t);
void jsr(struct nestor *, uint16_t);
void lda(struct nestor *, uint8_t*);
void ldx(struct nestor *, uint8_t*);
void ldy(struct nestor *, uint8_t*);
void lsr(struct nestor *, uint8_t*);
void nop(struct nestor *);
void ora(struct nestor *, uint8_t*);
void pha(struct nestor *);
void php(struct nestor *);
void pla(struct nestor *);
void plp(struct nestor *);
void rol(struct nestor *, uint8_t*);
void ror(struct nestor *, uint8_t*);
void rti(struct nestor *);
void rts(struct nestor *);
void sbc(struct nestor *, uint8_t*);
void sec(struct nestor *);
void sed(struct nestor *);
void sei(struct nestor *);
void sta(struct nestor *, uint8_t*);
void stx(struct nestor *, uint8_t*);
void sty(struct nestor *, uint8_t*);
void tax(struct nestor *);
void tay(struct nestor *);
void tsx(struct nestor *);
void txa(struct nestor *);
void txs(struct nestor *);
void tya(struct nestor *);




#endif

