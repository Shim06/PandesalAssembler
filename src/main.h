#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "tokenizer.h"
#include "hashtable.h"
#include "isa.h"

typedef enum
{
	ERR_OK = 0,
	ERR_FILE_NOT_FOUND,
	ERR_MEMORY_ALLOCATION_FAILED,
	ERR_INVALID_INSTRUCTION,
	ERR_INVALID_SYMBOL,
	ERR_SYNTAX,
	ERR_UNKNOWN,
} ErrorCode;

int first_pass(TokenList* list, HashTable* symbol_table);
int second_pass(TokenList* list, HashTable* symbol_table, HashTable* mnemonic_table, FILE* file);
void process_line(Token* tokens, int token_count, uint16_t* pc, int line, HashTable* symbol_table);
int generate_binary(Token* tokens, int token_count, HashTable* symbol_table, HashTable* mnemonic_table, FILE* file);
uint16_t parse_address(const char* str);

uint8_t get_immediate_value(const char* text);