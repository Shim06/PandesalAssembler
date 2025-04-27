#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//strList* init_str_list();
char* read_file_to_string(const char* filename);
FILE* open_file(const char* filename);

unsigned long djb2_hash(const unsigned char* str);