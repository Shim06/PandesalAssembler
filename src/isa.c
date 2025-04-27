#include "isa.h"

const InstructionMnemonic instruction_mnemonic_map[MNEMONIC_COUNT] = {
    {"NOP", MNEM_NOP}, {"LDA", MNEM_LDA}, {"STA", MNEM_STA}, {"JMP", MNEM_JMP}, {"ADC", MNEM_ADC},
    {"AND", MNEM_AND}, {"ORA", MNEM_ORA}, {"XOR", MNEM_XOR}, {"NOT", MNEM_NOT}, {"LDX", MNEM_LDX},
    {"LDY", MNEM_LDY}, {"TAX", MNEM_TAX}, {"TAY", MNEM_TAY}, {"STX", MNEM_STX}, {"STY", MNEM_STY},
    {"TSX", MNEM_TSX}, {"TXA", MNEM_TXA}, {"TXS", MNEM_TXS}, {"TYA", MNEM_TYA}, {"SBC", MNEM_SBC},
    {"PHA", MNEM_PHA}, {"PHC", MNEM_PHC}, {"CLC", MNEM_CLC}, {"HLT", MNEM_HLT}, {"RTS", MNEM_RTS},
    {"CLV", MNEM_CLV}, {"BCC", MNEM_BCC}, {"BCS", MNEM_BCS}, {"BEQ", MNEM_BEQ}, {"BMI", MNEM_BMI},
    {"BNE", MNEM_BNE}, {"BPL", MNEM_BPL}, {"BVC", MNEM_BVC}, {"BVS", MNEM_BVS}, {"INX", MNEM_INX},
    {"INY", MNEM_INY}, {"SEC", MNEM_SEC}, {"DEX", MNEM_DEX}, {"DEY", MNEM_DEY}, {"CMP", MNEM_CMP},
    {"PLA", MNEM_PLA}
};  

const uint16_t opcode_table[MNEMONIC_COUNT][ADDR_COUNT] = {
    [MNEM_NOP] = 
    {
        [ADDR_IMPLIED] = NOP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_LDA] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = LDA_IMM,
        [ADDR_ABSOLUTE] = LDA_ABS
    },
    [MNEM_STA] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = STA_ABS
    },
    [MNEM_JMP] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = JMP_ABS
    },
    [MNEM_ADC] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = ADC_IMM,
        [ADDR_ABSOLUTE] = ADC_ABS
    },
    [MNEM_AND] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = AND_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_ORA] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = ORA_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_XOR] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = XOR_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_NOT] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = NOT_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_LDX] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = LDX_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_LDY] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = LDY_IMM,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_TAX] = 
    {
        [ADDR_IMPLIED] = TAX_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_TAY] = 
    {
        [ADDR_IMPLIED] = TAY_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_STX] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = STX_ABS
    },
    [MNEM_STY] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = STY_ABS
    },
    [MNEM_TSX] = 
    {
        [ADDR_IMPLIED] = TSX_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_TXA] = 
    {
        [ADDR_IMPLIED] = TXA_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_TXS] = 
    {
        [ADDR_IMPLIED] = TXS_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_TYA] = 
    {
        [ADDR_IMPLIED] = TYA_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_SBC] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = SBC_IMM,
        [ADDR_ABSOLUTE] = SBC_ABS
    },
    [MNEM_PHA] = 
    {
        [ADDR_IMPLIED] = PHA_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_PLA] = 
    {
        [ADDR_IMPLIED] = PLA_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_PHC] =
    {
        [ADDR_IMPLIED] = PHC_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_CLC] = 
    {
        [ADDR_IMPLIED] = CLC_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_HLT] = 
    {
        [ADDR_IMPLIED] = HLT,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_RTS] = 
    {
        [ADDR_IMPLIED] = RTS_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_CLV] = 
    {
        [ADDR_IMPLIED] = CLV_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_BCC] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BCC_ABS
    },
    [MNEM_BCS] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BCS_ABS
    },
    [MNEM_BEQ] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BEQ_ABS
    },
    [MNEM_BMI] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BMI_ABS
    },
    [MNEM_BNE] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BNE_ABS
    },
    [MNEM_BPL] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BPL_ABS
    },
    [MNEM_BVC] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BVC_ABS
    },
    [MNEM_BVS] = 
    {
        [ADDR_IMPLIED] = INVALID_OPCODE,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = BVS_ABS
    },
    [MNEM_INX] = 
    {
        [ADDR_IMPLIED] = INX_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_INY] = 
    {
        [ADDR_IMPLIED] = INY_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_SEC] = 
    {
        [ADDR_IMPLIED] = SEC_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_DEX] = 
    {
        [ADDR_IMPLIED] = DEX_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_DEY] = 
    {
        [ADDR_IMPLIED] = DEY_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    },
    [MNEM_CMP] = 
    {
        [ADDR_IMPLIED] = CMP_IMP,
        [ADDR_IMMEDIATE] = INVALID_OPCODE,
        [ADDR_ABSOLUTE] = INVALID_OPCODE
    }
};


uint8_t get_instruction_size(AddressingMode mode)
{
    switch (mode)
    {
    case ADDR_IMPLIED:
        return 1;
    case ADDR_IMMEDIATE:
    case ADDR_ZEROPAGE:
    case ADDR_ZEROPAGEX:
    case ADDR_ZEROPAGEY:
    case ADDR_INDIRECTX:
    case ADDR_INDIRECTY:
    case ADDR_RELATIVE:
        return 2;
    case ADDR_ABSOLUTE:
    case ADDR_ABSOLUTEX:
    case ADDR_ABSOLUTEY:
    case ADDR_INDIRECT:
        return 3;
    }

    return 0;
}

AddressingMode get_addressing_mode(Token* tokens, int start, int count)
{
    //printf("start: %i, count: %i\n", start, count);
    if (start == count) return ADDR_IMPLIED;

    TokenType op1 = tokens[start].type;
    Token op2 = (start + 1 < count) ? tokens[start + 1] : (Token){ .type = TOKEN_UNKNOWN };
    Token op3 = (start + 2 < count) ? tokens[start + 2] : (Token){ .type = TOKEN_UNKNOWN };

    if (op1 == TOKEN_IMMEDIATE) return ADDR_IMMEDIATE;

    // Absolute addressing
    if (op1 == TOKEN_ADDRESS && op2.type == TOKEN_COMMA && op3.text == "X") return ADDR_ABSOLUTEX;
    if (op1 == TOKEN_ADDRESS && op2.type == TOKEN_COMMA && op3.text == "Y") return ADDR_ABSOLUTEY;
    if (op1 == TOKEN_ADDRESS) return ADDR_ABSOLUTE;
    if (op1 == TOKEN_IDENTIFIER) return ADDR_ABSOLUTE;

    // Todo: Indirect (X,Y), ZeroPage (X, Y), 

    return -1;
}

uint16_t get_opcode(Mnemonic mnemonic, AddressingMode addressing_mode)
{
    return opcode_table[mnemonic][addressing_mode];
}

HashTable* initialize_mnemonic_table(void)
{
    HashTable* table = create_hash_table();
    if (!table) return NULL;
    size_t map_size = sizeof(instruction_mnemonic_map) / sizeof(instruction_mnemonic_map[0]);
    for (size_t i = 0, size = map_size; i < size; i++)
    {
        if (insert(table, instruction_mnemonic_map[i].str, instruction_mnemonic_map[i].mnemonic_enum) != 0)
            return NULL;
    }
    return table;
}