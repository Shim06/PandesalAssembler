#include "utils.h"

char* read_file_to_string(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "\nFailed to open file\n");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	rewind(file);

	if (length < 0)
	{
		fprintf(stderr, "\nftell failed\n");
		fclose(file);
		return NULL;
	}

	if ((unsigned long)length >= SIZE_MAX)
	{
		fprintf(stderr, "\nFile too large to handle\n");
		fclose(file);
		return NULL;
	}

	size_t size = (size_t)length;

	char* buffer = malloc(size + 1);
	if (!buffer)
	{
		fprintf(stderr, "Memory Allocation failed");
		fclose(file);
		return NULL;
	}

	size_t read_size = fread(buffer, 1, size, file);
	buffer[read_size] = '\0';

	fclose(file);
	return buffer;
}

FILE* open_file(const char* filename)
{
	FILE* file = fopen(filename, "r");
	if (!file)
	{
		fprintf(stderr, "\nFailed to open file\n");
		return NULL;
	}
	return file;
}

unsigned long djb2_hash(const unsigned char* str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c;
	return hash;
}
