#include "nestor.h"

#define NES_ROM_HEADER_SIZE 16

NES_DEF(adc, immediate);
NES_DEF(adc, zero_page);
NES_DEF(adc, zero_page_x);
NES_DEF(adc, absolute);
NES_DEF(adc, absolute_x);
NES_DEF(adc, absolute_y);
NES_DEF(adc, indirect_x);
NES_DEF(adc, indirect_y);

NES_DEF(and, immediate);
NES_DEF(and, zero_page);
NES_DEF(and, zero_page_x);
NES_DEF(and, absolute);
NES_DEF(and, absolute_x);
NES_DEF(and, absolute_y);
NES_DEF(and, indirect_x);
NES_DEF(and, indirect_y);

NES_DEF(asl, accumulator);
NES_DEF(asl, zero_page);
NES_DEF(asl, zero_page_x);
NES_DEF(asl, absolute);
NES_DEF(asl, absolute_x);

NES_DEF(bcc, relative);
NES_DEF(bcs, relative);
NES_DEF(beq, relative);
NES_DEF(bmi, relative);
NES_DEF(bne, relative);
NES_DEF(bpl, relative);
NES_DEF(bvc, relative);
NES_DEF(bvs, relative);

NES_DEF(nes_brk, implied);

NES_DEF(bit, zero_page);
NES_DEF(bit, absolute);

NES_DEF(clc, implied);
NES_DEF(cld, implied);
NES_DEF(cli, implied);
NES_DEF(clv, implied);

NES_DEF(cmp, immediate);
NES_DEF(cmp, zero_page);
NES_DEF(cmp, zero_page_x);
NES_DEF(cmp, absolute);
NES_DEF(cmp, absolute_x);
NES_DEF(cmp, absolute_y);
NES_DEF(cmp, indirect_x);
NES_DEF(cmp, indirect_y);

NES_DEF(cpx, immediate);
NES_DEF(cpx, zero_page);
NES_DEF(cpx, absolute);

NES_DEF(cpy, immediate);
NES_DEF(cpy, zero_page);
NES_DEF(cpy, absolute);

NES_DEF(dec, zero_page);
NES_DEF(dec, zero_page_x);
NES_DEF(dec, absolute);
NES_DEF(dec, absolute_x);

NES_DEF(dex, implied);
NES_DEF(dey, implied);

NES_DEF(eor, immediate);
NES_DEF(eor, zero_page);
NES_DEF(eor, zero_page_x);
NES_DEF(eor, absolute);
NES_DEF(eor, absolute_x);
NES_DEF(eor, absolute_y);
NES_DEF(eor, indirect_x);
NES_DEF(eor, indirect_y);

NES_DEF(inc, zero_page);
NES_DEF(inc, zero_page_x);
NES_DEF(inc, absolute);
NES_DEF(inc, absolute_x);

NES_DEF(inx, implied);
NES_DEF(iny, implied);

NES_DEF(jmp, jmp_absolute);
NES_DEF(jmp, indirect);

NES_DEF(jsr, jmp_absolute);

NES_DEF(lda, immediate);
NES_DEF(lda, zero_page);
NES_DEF(lda, zero_page_x);
NES_DEF(lda, absolute);
NES_DEF(lda, absolute_x);
NES_DEF(lda, absolute_y);
NES_DEF(lda, indirect_x);
NES_DEF(lda, indirect_y);

NES_DEF(ldx, immediate);
NES_DEF(ldx, zero_page);
NES_DEF(ldx, zero_page_y);
NES_DEF(ldx, absolute);
NES_DEF(ldx, absolute_y);

NES_DEF(ldy, immediate);
NES_DEF(ldy, zero_page);
NES_DEF(ldy, zero_page_x);
NES_DEF(ldy, absolute);
NES_DEF(ldy, absolute_x);


NES_DEF(lsr, accumulator);
NES_DEF(lsr, zero_page);
NES_DEF(lsr, zero_page_x);
NES_DEF(lsr, absolute);
NES_DEF(lsr, absolute_x);

NES_DEF(nop, implied);

NES_DEF(ora, immediate);
NES_DEF(ora, zero_page);
NES_DEF(ora, zero_page_x);
NES_DEF(ora, absolute);
NES_DEF(ora, absolute_x);
NES_DEF(ora, absolute_y);
NES_DEF(ora, indirect_x);
NES_DEF(ora, indirect_y);

NES_DEF(pha, implied);
NES_DEF(php, implied);

NES_DEF(pla, implied);
NES_DEF(plp, implied);

NES_DEF(rol, accumulator);
NES_DEF(rol, zero_page);
NES_DEF(rol, zero_page_x);
NES_DEF(rol, absolute);
NES_DEF(rol, absolute_x);

NES_DEF(ror, accumulator);
NES_DEF(ror, zero_page);
NES_DEF(ror, zero_page_x);
NES_DEF(ror, absolute);
NES_DEF(ror, absolute_x);

NES_DEF(rti, implied);
NES_DEF(rts, implied);

NES_DEF(sbc, immediate);
NES_DEF(sbc, zero_page);
NES_DEF(sbc, zero_page_x);
NES_DEF(sbc, absolute);
NES_DEF(sbc, absolute_x);
NES_DEF(sbc, absolute_y);
NES_DEF(sbc, indirect_x);
NES_DEF(sbc, indirect_y);

NES_DEF(sec, implied);
NES_DEF(sed, implied);
NES_DEF(sei, implied);

NES_DEF(sta, zero_page);
NES_DEF(sta, zero_page_x);
NES_DEF(sta, absolute);
NES_DEF(sta, absolute_x);
NES_DEF(sta, absolute_y);
NES_DEF(sta, indirect_x);
NES_DEF(sta, indirect_y);

NES_DEF(stx, zero_page);
NES_DEF(stx, zero_page_y);
NES_DEF(stx, absolute);

NES_DEF(sty, zero_page);
NES_DEF(sty, zero_page_x);
NES_DEF(sty, absolute);

NES_DEF(tax, implied);
NES_DEF(tay, implied);

NES_DEF(tsx, implied);
NES_DEF(txa, implied);
NES_DEF(txs, implied);
NES_DEF(tya, implied);



struct nestor nestor_init()
{
	struct nestor nes = (struct nestor){
		.opcodes = {NULL},
		.regs = {
			.status = 0x24,
			.sp = 0x00FD
		}
	};

	nes.opcodes[ADC_IMMEDIATE] = nes_call_adc_immediate;
	nes.opcodes[ADC_ZERO_PAGE] = nes_call_adc_zero_page;
	nes.opcodes[ADC_ZERO_PAGE_X] = nes_call_adc_zero_page_x;
	nes.opcodes[ADC_ABSOLUTE] = nes_call_adc_absolute;
	nes.opcodes[ADC_ABSOLUTE_X] = nes_call_adc_absolute_x;
	nes.opcodes[ADC_ABSOLUTE_Y] = nes_call_adc_absolute_y;
	nes.opcodes[ADC_INDIRECT_X] = nes_call_adc_indirect_x;
	nes.opcodes[ADC_INDIRECT_Y] = nes_call_adc_indirect_y;

	nes.opcodes[AND_IMMEDIATE] = nes_call_and_immediate;
	nes.opcodes[AND_ZERO_PAGE] = nes_call_and_zero_page;
	nes.opcodes[AND_ZERO_PAGE_X] = nes_call_and_zero_page_x;
	nes.opcodes[AND_ABSOLUTE] = nes_call_and_absolute;
	nes.opcodes[AND_ABSOLUTE_X] = nes_call_and_absolute_x;
	nes.opcodes[AND_ABSOLUTE_Y] = nes_call_and_absolute_y;
	nes.opcodes[AND_INDIRECT_X] = nes_call_and_indirect_x;
	nes.opcodes[AND_INDIRECT_Y] = nes_call_and_indirect_y;

	nes.opcodes[ASL_ACCUMULATOR] = nes_call_asl_accumulator;
	nes.opcodes[ASL_ZERO_PAGE] = nes_call_asl_zero_page;
	nes.opcodes[ASL_ZERO_PAGE_X] = nes_call_asl_zero_page_x;
	nes.opcodes[ASL_ABSOLUTE] = nes_call_asl_absolute;
	nes.opcodes[ASL_ABSOLUTE_X] = nes_call_asl_absolute_x;

	nes.opcodes[BCC] = nes_call_bcc_relative;
	nes.opcodes[BCS] = nes_call_bcs_relative;
	nes.opcodes[BEQ] = nes_call_beq_relative;
	nes.opcodes[BMI] = nes_call_bmi_relative;
	nes.opcodes[BNE] = nes_call_bne_relative;
	nes.opcodes[BPL] = nes_call_bpl_relative;
	nes.opcodes[BVC] = nes_call_bvc_relative;
	nes.opcodes[BVS] = nes_call_bvs_relative;

	nes.opcodes[BRK] = nes_call_nes_brk_implied;
	
	nes.opcodes[BIT_ABSOLUTE] = nes_call_bit_absolute;
	nes.opcodes[BIT_ZERO_PAGE] = nes_call_bit_zero_page;

	nes.opcodes[CLC] = nes_call_clc_implied;
	nes.opcodes[CLD] = nes_call_cld_implied;
	nes.opcodes[CLI] = nes_call_cli_implied;
	nes.opcodes[CLV] = nes_call_clv_implied;

	nes.opcodes[CMP_IMMEDIATE] = nes_call_cmp_immediate;
	nes.opcodes[CMP_ZERO_PAGE] = nes_call_cmp_zero_page;
	nes.opcodes[CMP_ZERO_PAGE_X] = nes_call_cmp_zero_page_x;
	nes.opcodes[CMP_ABSOLUTE] = nes_call_cmp_absolute;
	nes.opcodes[CMP_ABSOLUTE_X] = nes_call_cmp_absolute_x;
	nes.opcodes[CMP_ABSOLUTE_Y] = nes_call_cmp_absolute_y;
	nes.opcodes[CMP_INDIRECT_X] = nes_call_cmp_indirect_x;
	nes.opcodes[CMP_INDIRECT_Y] = nes_call_cmp_indirect_y;

	nes.opcodes[CPX_IMMEDIATE] = nes_call_cpx_immediate; 
	nes.opcodes[CPX_ZERO_PAGE] = nes_call_cpx_zero_page;
	nes.opcodes[CPX_ABSOLUTE] = nes_call_cpx_absolute;

	nes.opcodes[CPY_IMMEDIATE] = nes_call_cpy_immediate; 
	nes.opcodes[CPY_ZERO_PAGE] = nes_call_cpy_zero_page;
	nes.opcodes[CPY_ABSOLUTE] = nes_call_cpy_absolute;

	nes.opcodes[DEC_ZERO_PAGE] = nes_call_dec_zero_page;
	nes.opcodes[DEC_ZERO_PAGE_x] = nes_call_dec_zero_page_x;
	nes.opcodes[DEC_ABSOLUTE] = nes_call_dec_absolute;
	nes.opcodes[DEC_ABSOLUTE_X] = nes_call_dec_absolute_x;

	nes.opcodes[DEX] = nes_call_dex_implied;
	nes.opcodes[DEY] = nes_call_dey_implied;

	nes.opcodes[EOR_IMMEDIATE] = nes_call_eor_immediate;
	nes.opcodes[EOR_ZERO_PAGE] = nes_call_eor_zero_page;
	nes.opcodes[EOR_ZERO_PAGE_X] = nes_call_eor_zero_page_x; 
	nes.opcodes[EOR_ABSOLUTE] = nes_call_eor_absolute;
	nes.opcodes[EOR_ABSOLUTE_X] = nes_call_eor_absolute_x;
	nes.opcodes[EOR_ABSOLUTE_Y] = nes_call_eor_absolute_y;
	nes.opcodes[EOR_INDIRECT_X] = nes_call_eor_indirect_x;
	nes.opcodes[EOR_INDIRECT_Y] = nes_call_eor_indirect_y;

	nes.opcodes[INC_ZERO_PAGE] = nes_call_inc_zero_page;
	nes.opcodes[INC_ZERO_PAGE_X] = nes_call_inc_zero_page_x;
	nes.opcodes[INC_ABSOLUTE] = nes_call_inc_absolute;
	nes.opcodes[INC_ABSOLUTE_X] = nes_call_inc_absolute_x;

	nes.opcodes[INX] = nes_call_inx_implied;
	nes.opcodes[INY] = nes_call_iny_implied;

	nes.opcodes[JMP_ABSOLUTE] = nes_call_jmp_jmp_absolute;
	nes.opcodes[JMP_INDIRECT] = nes_call_jmp_indirect;

	nes.opcodes[JSR] = nes_call_jsr_jmp_absolute;

	nes.opcodes[LDA_IMMEDIATE] = nes_call_lda_immediate;
	nes.opcodes[LDA_ZERO_PAGE] = nes_call_lda_zero_page;
	nes.opcodes[LDA_ZERO_PAGE_x] = nes_call_lda_zero_page_x;
	nes.opcodes[LDA_ABSOLUTE] = nes_call_lda_absolute;
	nes.opcodes[LDA_ABSOLUTE_X] = nes_call_lda_absolute_x;
	nes.opcodes[LDA_ABSOLUTE_Y] = nes_call_lda_absolute_y; 
	nes.opcodes[LDA_INDIRECT_X] = nes_call_lda_indirect_x;
	nes.opcodes[LDA_INDIRECT_Y] = nes_call_lda_indirect_y;

	nes.opcodes[LDX_IMMEDIATE] = nes_call_ldx_immediate;
	nes.opcodes[LDX_ZERO_PAGE] = nes_call_ldx_zero_page;
	nes.opcodes[LDX_ZERO_PAGE_Y] = nes_call_ldx_zero_page_y; 
	nes.opcodes[LDX_ABSOLUTE] = nes_call_ldx_absolute;
	nes.opcodes[LDX_ABSOLUTE_Y] = nes_call_ldx_absolute_y;

	nes.opcodes[LDY_IMMEDIATE] = nes_call_ldy_immediate;
	nes.opcodes[LDY_ZERO_PAGE] = nes_call_ldy_zero_page;
	nes.opcodes[LDY_ZERO_PAGE_X] = nes_call_ldy_zero_page_x; 
	nes.opcodes[LDY_ABSOLUTE] = nes_call_ldy_absolute;
	nes.opcodes[LDY_ABSOLUTE_X] = nes_call_ldy_absolute_x;

	nes.opcodes[LSR_ACCUMULATOR] = nes_call_lsr_accumulator;
	nes.opcodes[LSR_ZERO_PAGE] = nes_call_lsr_zero_page;
	nes.opcodes[LSR_ZERO_PAGE_X] = nes_call_lsr_zero_page_x; 
	nes.opcodes[LSR_ABSOLUTE] = nes_call_lsr_absolute;
	nes.opcodes[LSR_ABSOLUTE_X] = nes_call_lsr_absolute_x;

	nes.opcodes[NOP] = nes_call_nop_implied;

	nes.opcodes[ORA_IMMEDIATE] = nes_call_ora_immediate;
	nes.opcodes[ORA_ZERO_PAGE] = nes_call_ora_zero_page;
	nes.opcodes[ORA_ZERO_PAGE_X] = nes_call_ora_zero_page_x; 
	nes.opcodes[ORA_ABSOLUTE] = nes_call_ora_absolute;
	nes.opcodes[ORA_ABSOLUTE_X] = nes_call_ora_absolute_x;
	nes.opcodes[ORA_ABSOLUTE_Y] = nes_call_ora_absolute_y;
	nes.opcodes[ORA_INDIRECT_X] = nes_call_ora_indirect_x;
	nes.opcodes[ORA_INDIRECT_Y] = nes_call_ora_indirect_y; 

	nes.opcodes[PHA] = nes_call_pha_implied;
	nes.opcodes[PHP] = nes_call_php_implied;

	nes.opcodes[PLA] = nes_call_pla_implied;
	nes.opcodes[PLP] = nes_call_plp_implied;

	nes.opcodes[ROL_ACCUMULATOR] = nes_call_rol_accumulator;
	nes.opcodes[ROL_ZERO_PAGE] = nes_call_rol_zero_page; 
	nes.opcodes[ROL_ZERO_PAGE_X] = nes_call_rol_zero_page_x;
	nes.opcodes[ROL_ABSOLUTE] = nes_call_rol_absolute;
	nes.opcodes[ROL_ABSOLUTE_X] = nes_call_rol_absolute_x;

	nes.opcodes[ROR_ACCUMULATOR] = nes_call_ror_accumulator;
	nes.opcodes[ROR_ZERO_PAGE] = nes_call_ror_zero_page; 
	nes.opcodes[ROR_ZERO_PAGE_X] = nes_call_ror_zero_page_x;
	nes.opcodes[ROR_ABSOLUTE] = nes_call_ror_absolute;
	nes.opcodes[ROR_ABSOLUTE_X] = nes_call_ror_absolute_x;

	nes.opcodes[RTI] = nes_call_rti_implied;
	nes.opcodes[RTS] = nes_call_rts_implied;

	nes.opcodes[SBC_IMMEDIATE] = nes_call_sbc_immediate; 
	nes.opcodes[SBC_ZERO_PAGE] = nes_call_sbc_zero_page;
	nes.opcodes[SBC_ZERO_PAGE_X] = nes_call_sbc_zero_page_x;
	nes.opcodes[SBC_ABSOLUTE] = nes_call_sbc_absolute;
	nes.opcodes[SBC_ABSOLUTE_X] = nes_call_sbc_absolute_x;
	nes.opcodes[SBC_ABSOLUTE_Y] = nes_call_sbc_absolute_y;
	nes.opcodes[SBC_INDIRECT_X] = nes_call_sbc_indirect_x; 
	nes.opcodes[SBC_INDIRECT_Y] = nes_call_sbc_indirect_y;

	nes.opcodes[SEC] = nes_call_sec_implied;
	nes.opcodes[SED] = nes_call_sed_implied;
	nes.opcodes[SEI] = nes_call_sei_implied;

	nes.opcodes[STA_ZERO_PAGE] = nes_call_sta_zero_page;
	nes.opcodes[STA_ZERO_PAGE_X] = nes_call_sta_zero_page_x;
	nes.opcodes[STA_ABSOLUTE] = nes_call_sta_absolute;
	nes.opcodes[STA_ABSOLUTE_X] = nes_call_sta_absolute_x; 
	nes.opcodes[STA_ABSOLUTE_Y] = nes_call_sta_absolute_y;
	nes.opcodes[STA_INDIRECT_X] = nes_call_sta_indirect_x;
	nes.opcodes[STA_INDIRECT_Y] = nes_call_sta_indirect_y;

	nes.opcodes[STX_ZERO_PAGE] = nes_call_stx_zero_page;
	nes.opcodes[STX_ZERO_PAGE_Y] = nes_call_stx_zero_page_y;
	nes.opcodes[STX_ABSOLUTE] = nes_call_stx_absolute;

	nes.opcodes[STY_ZERO_PAGE] = nes_call_sty_zero_page;
	nes.opcodes[STY_ZERO_PAGE_X] = nes_call_sty_zero_page_x;
	nes.opcodes[STY_ABSOLUTE] = nes_call_sty_absolute; 

	nes.opcodes[TAX] = nes_call_tax_implied;
	nes.opcodes[TAY] = nes_call_tay_implied;
	nes.opcodes[TSX] = nes_call_tsx_implied; 
	nes.opcodes[TXA] = nes_call_txa_implied;
	nes.opcodes[TXS] = nes_call_txs_implied;
	nes.opcodes[TYA] = nes_call_tya_implied; 

	return nes;
}


void emulate(struct nestor * nes)
{
	//take instruction code

	uint8_t op = nes->memory[nes->regs.pc];

	if (nes->opcodes[op] != NULL) {

#ifdef NESTOR_DEBUG
		nes->opcodes[op](nes);
		DBG_CPU(nes);
	} else {
		DBGF("[M:%02x]: %02x not found\n", nes->regs.pc, op);
		getchar();
	}
	//printf("%x - %x\n", nes->memory[0x0000], nes->memory[0x0001]);
#else 
	nes->opcodes[op](nes);
	}
#endif
#ifdef NESTOR_BREAK
	getchar();
#endif

	nes->regs.pc++;
}



uint8_t nestor_set_byte(struct nestor * nes, uint16_t p_byte, uint8_t val)
{
	//get as a little endian address
	nes->memory[p_byte^0x1] = val; 
}


#define INES_IS_VERTICAL_MIRROR(flag) ((flag & 0x09) == 0x0)
#define INES_IS_HORIZONTAL_MIRROR(flag) ((flag & 0x09) == 0x1)
#define INES_IS_FOUR_SCREEN(flag) ((flag & 0x08) == 0x8)
#define PRG_ROM1_OFFSET 0x8000
#define PRG_ROM2_OFFSET 0xC000

#define RESET_INTERRUPT 0xFFFC
#define IRQ_INTERRUPT 0xFFFE
#define NMI_INTERRPUT 0xFFFA


int nestor_cartridge(struct nestor *nes, char *game) 
{
	FILE *f = fopen(game, "r");
	
	fseek(f, 0, SEEK_END);
	int game_size = ftell(f);
	rewind(f);

	//if (game_size > NES_MEM_SIZE)
	//	return 1;


	uint8_t header_buffer[NES_ROM_HEADER_SIZE];  

	fread(header_buffer, sizeof(uint8_t), NES_ROM_HEADER_SIZE, f);

	if (header_buffer[1] != 0x45 	//N
		|| header_buffer[0] != 0x4e	//E
		|| header_buffer[3] != 0x1a	//S
		|| header_buffer[2] != 0x53)	//
		return 1;


	int prg_rom_size = (int)header_buffer[5] * 16384;

	fread(nes->memory + PRG_ROM1_OFFSET, sizeof(uint8_t), prg_rom_size, f);
	memcpy(nes->memory + PRG_ROM2_OFFSET, nes->memory + PRG_ROM1_OFFSET, prg_rom_size);

	int chr_rom_size = (int)header_buffer[4] * 8192;
	
	fread(nes->video.memory, sizeof(uint8_t), chr_rom_size, f);


	uint8_t flag_6 = header_buffer[7];

	if (INES_IS_VERTICAL_MIRROR(flag_6))
		nes->video.mirror = VERTICAL;
	else if (INES_IS_HORIZONTAL_MIRROR(flag_6))
		nes->video.mirror = HORIZONTAL;
	else if (INES_IS_FOUR_SCREEN(flag_6))
		nes->video.mirror = FOUR;


	int prg_ram = flag_6 & 0x02;
	int trainer_store = flag_6 & 0x04;

	uint8_t flag_7 =header_buffer[6];

	uint8_t mapper_num = ((flag_6 & 0xF0) >> 8) | (flag_7 & 0xF0);

	int i;
	for (i=8; i < 16; i++)
		if (header_buffer[i] != 0x0) 
			return 1;

	nes->regs.pc = nes->memory[RESET_INTERRUPT] | (nes->memory[RESET_INTERRUPT + 1] << 8);

	return 0;
}


void set_irq(struct nestor *nes) 
{
	nes->regs.pc = (nes->memory[IRQ_INTERRUPT + 1] << 8) | nes->memory[IRQ_INTERRUPT];

}

void set_nmi(struct nestor *nes)
{
	nes->regs.pc = (nes->memory[NMI_INTERRPUT + 1] << 8) | nes->memory[NMI_INTERRPUT];
}

int nestor_events(struct nestor *nes) 
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return 1;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE) return 1;
		}
	}
	return 0;
}