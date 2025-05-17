#pragma once
#include <stdint.h>
#include <string.h>
#include "hashtable.h"
#include "tokenizer.h"

#define NOP 0x00
#define LDA_IMM 0x01
#define STA_ABS 0x02
#define JMP_ABS 0x03
#define ADC_IMM 0x04
#define AND_IMM 0x05
#define ORA_IMM 0x06
#define XOR_IMM 0x07
#define NOT_IMM 0x08
#define LDX_IMM 0x09
#define LDY_IMM 0x0A
#define TAX_IMP 0x0B
#define TAY_IMP 0x0C
#define STX_ABS 0x0D
#define STY_ABS 0x0E
#define TSX_IMP 0x0F
#define TXA_IMP 0x10
#define TXS_IMP 0x11
#define TYA_IMP 0x12
#define SBC_IMM 0x13
#define PHA_IMP 0x14
#define PLA_IMP 0x15
#define PHC_IMP 0x16
#define CLC_IMP 0x17
#define HLT 0x18
#define LDA_ABS 0x19
#define ADC_ABS 0x1A
#define RTS_IMP 0x1B
#define SBC_ABS 0x1C
#define CLV_IMP 0x1D
#define BCC_ABS 0x1E
#define BCS_ABS 0x1F
#define BEQ_ABS 0x20
#define BMI_ABS 0x21
#define BNE_ABS 0x22
#define BPL_ABS 0x23
#define BVC_ABS 0x24
#define BVS_ABS 0x25
#define INX_IMP 0x26
#define INY_IMP 0x27
#define SEC_IMP 0x28
#define DEX_IMP 0x29
#define DEY_IMP 0x2A
#define CMP_IMM 0x2B
#define ASL_A 0x2C
#define LSR_A 0x2D
#define ROL_A 0x2E
#define ROR_A 0x2F
#define ASL_ABS 0x30
#define LSR_ABS 0x31
#define ROL_ABS 0x32
#define ROR_ABS 0x33
#define CPX_IMP 0x34
#define CPY_IMP 0x35
#define CPX_ABS 0x36
#define CPY_ABS 0x37
#define LDX_ABS 0x38
#define LDY_ABS 0x39
#define LDA_ABSX 0x3A
#define LDA_ABSY 0x3B
#define AND_ABSX 0x3C
#define AND_ABSY 0x3D
#define STA_ABSX 0x3E
#define STA_ABSY 0x3F
#define INC_A 0x40
#define INC_ABS 0x41
#define STA_IND 0x42
#define ADC_ABSX 0x43
#define ADC_ABSY 0x44
#define ADD_IMM 0x45
#define SUB_IMM 0x46
#define ADD_ABS 0x47
#define SUB_ABS 0x48
#define INVALID_OPCODE 0xFFFF

typedef enum
{
	MNEM_NOP,
	MNEM_LDA,
	MNEM_STA,
	MNEM_JMP,
	MNEM_ADC,
	MNEM_AND,
	MNEM_ORA,
	MNEM_XOR,
	MNEM_NOT,
	MNEM_LDX,
	MNEM_LDY,
	MNEM_TAX,
	MNEM_TAY,
	MNEM_STX,
	MNEM_STY,
	MNEM_TSX,
	MNEM_TXA,
	MNEM_TXS,
	MNEM_TYA,
	MNEM_SBC,
	MNEM_PHA,
	MNEM_PLA,
	MNEM_PHC,
	MNEM_CLC,
	MNEM_HLT,
	MNEM_RTS,
	MNEM_CLV,
	MNEM_BCC,
	MNEM_BCS,
	MNEM_BEQ,
	MNEM_BMI,
	MNEM_BNE,
	MNEM_BPL,
	MNEM_BVC,
	MNEM_BVS,
	MNEM_INX,
	MNEM_INY,
	MNEM_SEC,
	MNEM_DEX,
	MNEM_DEY,
	MNEM_CMP,
	MNEM_ASL,
	MNEM_LSR,
	MNEM_ROL,
	MNEM_ROR,
	MNEM_CPX,
	MNEM_CPY,
	MNEM_INC,
	MNEM_ADD,
	MNEM_SUB,
	MNEMONIC_COUNT,
} Mnemonic;

typedef enum
{
	ADDR_ACCUMULATOR,
	ADDR_IMPLIED,
	ADDR_IMMEDIATE,
	ADDR_ABSOLUTE,
	ADDR_ZEROPAGE,
	ADDR_INDIRECT,
	ADDR_RELATIVE,
	ADDR_INDIRECTX,
	ADDR_INDIRECTY,
	ADDR_ABSOLUTEX,
	ADDR_ABSOLUTEY,
	ADDR_ZEROPAGEX,
	ADDR_ZEROPAGEY,
	ADDR_COUNT
} AddressingMode;

typedef struct
{
	char str[4];
	Mnemonic mnemonic_enum;
} InstructionMnemonic;


extern const InstructionMnemonic instruction_mnemonic_map[MNEMONIC_COUNT];
extern const uint16_t opcode_table[MNEMONIC_COUNT][ADDR_COUNT];

HashTable* initialize_mnemonic_table(void);
uint8_t is_register(const char* str);
AddressingMode get_addressing_mode(Token* tokens, int start, int count);
uint8_t get_instruction_size(AddressingMode mode);
uint16_t get_opcode(Mnemonic mnemonic, AddressingMode addrmode);