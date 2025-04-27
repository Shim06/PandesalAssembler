#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "utils.h"

#define INITIAL_CAPACITY 2048
#define LOAD_FACTOR 0.7

typedef struct
{
	char* key;
	int value;
} Bucket;

typedef struct
{
	Bucket* buckets;
	size_t capacity;
	size_t size;
} HashTable;

HashTable* create_hash_table(void);
int resize(HashTable* table);
int insert(HashTable* table, const char* key, const int value);
int insert_with_hash(HashTable* table, const unsigned long hash, const int value);
int* search(HashTable* table, const char* key);