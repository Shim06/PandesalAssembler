#include "main.h"

int main(int argc, char* argv[])
{
	char* input_filename = NULL;
	input_filename = "temp.asm";
	char* output_filename = "temp.bin";	

	/*if (argc < 2) 
	{
		fprintf(stderr, "Usage: %s <input file> [-o <output file>]\n", argv[0]);
		return ERR_SYNTAX;
	}

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			if (i + 1 < argc)
			{
				output_filename = argv[i + 1];
				i++;
			}
			else
			{
				fprintf(stderr, "Error: -o requires a filename.\n");
				return ERR_SYNTAX;
			}
		}
		else
		{
			if (!input_filename)
			{
				input_filename = argv[i];
			}
			else
			{
				fprintf(stderr, "Error: Multiple input files specified.\n");
				return ERR_SYNTAX;
			}
		}
	}
	if (!input_filename) { fprintf(stderr, "Error: No input file specified.\n"); return ERR_SYNTAX; }*/

	// Read assembly file and tokenize
	int error;
	TokenList list;

	char* source = read_file_to_string(input_filename);
	if (!source) return ERR_FILE_NOT_FOUND;
	HashTable* symbol_table = create_hash_table();
	HashTable* mnemonic_table = initialize_mnemonic_table();
	if (!symbol_table || !mnemonic_table) { free(source); return ERR_MEMORY_ALLOCATION_FAILED; }

	init_token_list(&list);
	tokenize(source, &list, mnemonic_table);
	free(source);

	printf("First pass:\n");
	if ((error = first_pass(&list, symbol_table)) != ERR_OK) return error;

	FILE* output_file = fopen(output_filename, "wb");
	if (!output_file) { fprintf(stderr, "Could not open output file.\n"); return ERR_FILE_NOT_FOUND; }
	printf("\nSecond pass:\n");
	if ((error = second_pass(&list, symbol_table, mnemonic_table, output_file)) != ERR_OK)
	{
		fclose(output_file);
		remove(output_filename);
		return error;
	}

	fclose(output_file);
	return 0;
}

int first_pass(TokenList* list, HashTable* symbol_table)
{
	uint16_t program_counter = 0x0000;
	int line_token_count = 0;
	Token line_tokens[10];
	int line_counter = list->tokens[0].line;
	for (size_t i = 0; i < list->count; i++)
	{
		Token tok = list->tokens[i];

		if (tok.line != line_counter && line_token_count > 0)
		{
			process_line(line_tokens, line_token_count, &program_counter, line_counter, symbol_table);
			
			for (int i = 0; i < line_token_count; i++)
			{
				printf("line %i: [%i] %s\n", line_tokens[i].line, line_tokens[i].type, line_tokens[i].text);
			}
			printf("\n");
			

			line_token_count = 0;
			line_counter = tok.line;
		}
		line_tokens[line_token_count++] = tok;
		//printf("Line %i: [%i] %s\n", tok.line, tok.type, tok.text);
	}

	if (line_token_count > 0)
	{
		process_line(line_tokens, line_token_count, &program_counter, line_counter, symbol_table);
		
		for (int i = 0; i < line_token_count; i++)
		{
			printf("line %i: [%i] %s\n", line_tokens[i].line, line_tokens[i].type, line_tokens[i].text);
		}
		printf("\n");
		
	}
	line_token_count = 0;
	line_counter = 0;
	return 0;
}

int second_pass(TokenList* list, HashTable* symbol_table, HashTable* mnemonic_table, FILE* file)
{
	int line_token_count = 0;
	Token line_tokens[10];
	int line_counter = list->tokens[0].line;
	int error;
	for (size_t i = 0; i < list->count; i++)
	{
		Token tok = list->tokens[i];

		if (tok.line != line_counter && line_token_count > 0)
		{
			if ((error = generate_binary(line_tokens, line_token_count, symbol_table, mnemonic_table, file)) != ERR_OK)
			{
				switch (error)
				{
				case ERR_SYNTAX:
					fprintf(stderr, "\nError on line %d: Expected label or mnemonic, got '%s'\n", line_tokens[0].line, line_tokens[0].text);
					return error;

				case ERR_INVALID_INSTRUCTION:
					printf("\n%s\n", line_tokens[0].text);
					int* temp = search(mnemonic_table, line_tokens[0].text);
					if (!temp) return ERR_INVALID_INSTRUCTION;
					Mnemonic mnemonic = *temp;
					AddressingMode addrmode = get_addressing_mode(line_tokens, 1, line_token_count);

					uint16_t temp_opcode = get_opcode(mnemonic, addrmode);
					fprintf(stderr, "\nError on line %d: Invalid instruction '%s' with addressing mode '%i' \n", line_tokens[0].line, line_tokens[0].text, addrmode);
					return error;
				}
			}
			line_token_count = 0;
			line_counter = tok.line;
		}
		line_tokens[line_token_count++] = tok;
	}

	if (line_token_count > 0)
	{
		generate_binary(line_tokens, line_token_count, symbol_table, mnemonic_table, file);
		printf("\n");
	}
	return 0;
}

void process_line(Token* tokens, int token_count, uint16_t* pc, int line, HashTable* symbol_table)
{
	int tok_index = 0;
	if (tokens[0].type == TOKEN_LABEL)
	{
		printf("Inserted label %s: [$%.4x] in symbol table\n", tokens[0].text, *pc);
		insert(symbol_table, tokens[0].text, *pc);
		tok_index++;
	}

	while (tok_index < token_count)
	{
		Token tok = tokens[tok_index];
		if (tok.type == TOKEN_DIRECTIVE && strcmp(".byte", tok.text) == 0)
		{
			tok_index++;
			(*pc)++;
			continue;
		}

		if (tok.type != TOKEN_MNEMONIC)
		{
			tok_index++;
			continue;
		}

		AddressingMode addrmode = get_addressing_mode(tokens, tok_index + 1, token_count);
		//printf("Addressing mode: %i, Instruction size: %i\n", addrmode, get_instruction_size(addrmode));
		*pc += get_instruction_size(addrmode);
		tok_index++;
	}
}

int generate_binary(Token* tokens, int token_count, HashTable* symbol_table, HashTable* mnemonic_table, FILE* file)
{
	int tok_index = 0;
	if (tokens[0].type == TOKEN_NEWLINE || tokens[0].type == TOKEN_COMMENT) return 0;
	if (tokens[0].type != TOKEN_MNEMONIC && tokens[0].type != TOKEN_LABEL && tokens->type != TOKEN_DIRECTIVE) return ERR_SYNTAX;

	while (tok_index < token_count)
	{
		if (tokens[tok_index].type == TOKEN_DIRECTIVE)
		{
			if (strcmp(tokens[tok_index].text, ".byte") == 0)
			{
				uint8_t byte = get_immediate_value(tokens[tok_index + 1].text);
				fwrite(&byte, sizeof(byte), 1, file);
				printf("%.2x ", byte);
				tok_index += 2;
				continue;
			}
		}


		if (tokens[tok_index].type != TOKEN_MNEMONIC)
		{
			tok_index++;
			continue;
		}

		Token tok = tokens[tok_index];

		int* temp = search(mnemonic_table, tok.text);
		if (!temp) return ERR_INVALID_INSTRUCTION;
		Mnemonic mnemonic = *temp;
		AddressingMode addrmode = get_addressing_mode(tokens, tok_index + 1, token_count);

		uint16_t temp_opcode = get_opcode(mnemonic, addrmode);
		if (temp_opcode == 0xFFFF) return ERR_INVALID_INSTRUCTION;
		uint8_t opcode = (uint8_t)temp_opcode & 0x00FF;

		switch (addrmode)
		{
		case ADDR_ACCUMULATOR:
		case ADDR_IMPLIED:
			fwrite(&opcode, sizeof(opcode), 1, file);
			printf("%.2x ", opcode);
			tok_index++;
			break;
		case ADDR_IMMEDIATE:
			uint8_t imm_value = get_immediate_value(tokens[tok_index + 1].text + 1);
			fwrite(&opcode, sizeof(opcode), 1, file);
			fwrite(&imm_value, sizeof(imm_value), 1, file);
			printf("%.2x ", opcode);
			printf("%.2x ", imm_value);
			tok_index += 2;
			break;

		case ADDR_ABSOLUTE:
			fwrite(&opcode, sizeof(opcode), 1, file);
			printf("%.2x ", opcode);

			uint16_t address = 0x0000;
			uint8_t op1 = 0;
			uint8_t op2 = 0;
			if (tokens[tok_index + 1].type == TOKEN_ADDRESS)
			{
				address = parse_address(tokens[tok_index + 1].text);
				op1 = address & 0x00FF;
				op2 = (address >> 8) & 0x00FF;
			}
			else if (tokens[tok_index + 1].type == TOKEN_IDENTIFIER)
			{
				address = *search(symbol_table, tokens[tok_index + 1].text);
				if (!address) return ERR_INVALID_SYMBOL;
				op1 = address & 0x00FF;
				op2 = (address >> 8) & 0x00FF;
			}
			fwrite(&op1, sizeof(op1), 1, file);
			fwrite(&op2, sizeof(op2), 1, file);
			printf("%.2x ", op1);
			printf("%.2x ", op2);
			tok_index += 2;
			break;
		}
	}
	return 0;
}

uint16_t parse_address(const char* str)
{
	if (str[0] == '$')
	{
		return (uint16_t)strtol(str + 1, NULL, 16);
	}
	return 0xFFFF;
}

// Converts immediate value text to uint8_t | Ex: #$20
// '%' prefix - Binary | '$' prefix - Hexadecimal
uint8_t get_immediate_value(const char* val)
{
	if (val[0] == '%')
	{
		return (uint8_t)strtol((val + 1), NULL, 2);
	}
	else if (val[0] == '$')
	{
		return (uint8_t)strtol((val + 1), NULL, 16);
	}
	return (uint8_t)strtol((val), NULL, 10);
}
