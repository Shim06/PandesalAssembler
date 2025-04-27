#include "hashtable.h"


HashTable* create_hash_table(void)
{
	HashTable* table = malloc(sizeof(HashTable));
	if (!table)
	{
		fprintf(stderr, "\nFailed to allocate memory for hashtable.\n");
		return NULL;
	}

	table->capacity = INITIAL_CAPACITY;
	table->size = 0;
	table->buckets = calloc(table->capacity, sizeof(Bucket));
	if (!table->buckets)
	{
		fprintf(stderr, "\nFailed to allocate memory for hashtable.\n");
		free(table);
		return NULL;
	}

	return table;
}

int resize(HashTable* table)
{
	size_t old_capacity = table->capacity;
	Bucket* old_buckets = table->buckets;

	size_t new_capacity = table->capacity * 2;
	Bucket* new_buckets = calloc(new_capacity, sizeof(Bucket));
	if (!new_buckets)
	{
		fprintf(stderr, "Failed to allocate memory for hashtable.\n");
		return 1;
	}

	table->capacity = new_capacity;
	table->buckets = new_buckets;
	table->size = 0;

	for (int i = 0; i < old_capacity; i++)
	{
		if (old_buckets[i].key)
		{
			char* key = old_buckets[i].key;
			int value = old_buckets[i].value;

			unsigned long hashedkey = djb2_hash(key) % table->capacity;
			while (table->buckets[hashedkey].key)
			{
				hashedkey = (hashedkey + 1) % table->capacity;
			}

			table->buckets[hashedkey].key = key;
			table->buckets[hashedkey].value = value;
			table->size++;
		}
	}

	free(old_buckets);
	return 0;
}

int insert(HashTable* table, const char* key, const int value)
{
	if ((double)((table->size + 1) / table->capacity) > LOAD_FACTOR)
	{
		if (resize(table) != 0)
		{
			fprintf(stderr, "Failed to allocate memory.\n");
			return 1;
		}
	}

	unsigned long hashedkey = djb2_hash(key) % table->capacity;
	while (table->buckets[hashedkey].key)
	{
		if (strcmp(table->buckets[hashedkey].key, key) == 0)
		{
			table->buckets[hashedkey].value = value;
			return 0;
		}
		hashedkey = (hashedkey + 1) % table->capacity;
	}

	table->buckets[hashedkey].key = _strdup(key);
	table->buckets[hashedkey].value = value;
	table->size++;
	return 0;
}

int insert_with_hash(HashTable* table, const unsigned long hash, const int value)
{
	if ((double)((table->size + 1) / table->capacity) > LOAD_FACTOR)
	{
		if (resize(table) != 0)
		{
			fprintf(stderr, "Failed to allocate memory.\n");
			return 1;
		}
	}

	unsigned long hashedkey = hash % table->capacity;
	char hash_buffer[20];
	sprintf(hash_buffer, "%d", hash);
	while (table->buckets[hashedkey].key)
	{
		if (strcmp(table->buckets[hashedkey].key, hash_buffer) == 0)
		{
			table->buckets[hashedkey].value = value;
			return 0;
		}
		hashedkey = (hashedkey + 1) % table->capacity;
	}

	table->buckets[hashedkey].key = _strdup(hash_buffer);
	table->buckets[hashedkey].value = value;
	table->size++;
	return 0;
}

int* search(HashTable* table, const char* key)
{
	unsigned long hashedkey = djb2_hash(key) % table->capacity;
	while (table->buckets[hashedkey].key)
	{
		if (strcmp(table->buckets[hashedkey].key, key) == 0)
			return &table->buckets[hashedkey].value;
		hashedkey = (hashedkey + 1) % table->capacity;
	}

	return NULL;
}