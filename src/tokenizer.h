#pragma once
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashtable.h"

#define MAX_TOKEN_LEN 64
#define MAX_TOKENS_PER_LINE 16

typedef enum
{
	TOKEN_END,
	TOKEN_LABEL,
	TOKEN_MNEMONIC,
	TOKEN_IMMEDIATE,
	TOKEN_ADDRESS,
	TOKEN_INDIRECT,
	TOKEN_DIRECTIVE,
	TOKEN_COMMA,
	TOKEN_NEWLINE,
	TOKEN_COMMENT,
	TOKEN_IDENTIFIER,
	TOKEN_UNKNOWN
} TokenType;

typedef struct
{
	TokenType type;
	char text[MAX_TOKEN_LEN];
	int line;
} Token;

typedef struct
{
	Token *tokens;
	size_t count;
	size_t capacity;
} TokenList;

typedef struct
{
	const char* source;
	size_t pos;
	int line;
} Tokenizer;


char peek(Tokenizer* tz);
char advance(Tokenizer* tz);

void init_token_list(TokenList* list);
void free_token_list(TokenList* list);
int add_token(TokenList* list, Token token);
void skip_whitespace(Tokenizer* tz);
Token read_token(Tokenizer* tz, HashTable* mnemonic_table);
void tokenize(const char* source, TokenList* list, HashTable* mnemonic_table);