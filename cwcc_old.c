#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// pointer = a "variable-like" reference that holds a memory address to another variable
// because some tasks are performed more easily with pointers
// * = indirection operator (value at address)

/*
int age = 21
printf("address of age: %p", &age) -> & operator is the address of operator
printf("value of age: %d", age)

To create a pointer, make sure the variable is the same type as the original
int *pAge = &age; -> the address of age and the value of pAge are the same

printf("address of age: %p", pAge) -> it is %p because the value is an address
printf("value of age: %p", &age) -> this is the same as above

To access the value of a pointer
printf("address of age: %d", *pAge) -> precede the variable with "*" which is the indirection operator

*/

long getFileSize(FILE *);
int getNumberOfLines(char[], long);
int getNumberOfWordsInFileBuffer(char[]);

// Final challenge is to aaccept piped input to C program
// how do we determine if to use pipe input?

// Dynamically allocate memory for string
// https://stackoverflow.com/questions/8164000/how-to-dynamically-allocate-memory-space-for-a-string-and-get-that-string-from-u

/**
Wc is a tool that returns
*/

int main(int argc, char *argv[])
{
	printf("Starting program");
	int ch = EOF;
	char *buffer = NULL;
	char *file_buffer = NULL;
	char *tmp = NULL;
	size_t size = 0;
	size_t index = 0;
	int CHUNKSIZE = 1024;
	bool reading_from_stdin = !isatty(fileno(stdin));
	char flag[sizeof(argv[1]) + 1];
	int size_of_file_name = sizeof(argv[2] ? argv[2] : argv[1]);
	long file_size;
	char file_name[size_of_file_name + 1];
	strcpy(file_name, argv[2] ? argv[2] : argv[1]);
	if (!reading_from_stdin && argv[2])
	{
		strcpy(flag, argv[1]);
	}
	else if (argv[1])
	{
		strcpy(flag, argv[1]);
	}
	else
	{
		strcpy(flag, "");
	}
	if (reading_from_stdin)
	{
		ch = getc(stdin);
		while (ch)
		{
			ch = getc(stdin);
			if (ch == EOF)
			{
				ch = 0;
			}
			if (size <= index)
			{
				size += CHUNKSIZE;
				tmp = realloc(buffer, size);
				if (!tmp)
				{
					free(buffer);
					buffer = NULL;
					break;
				}
				buffer = tmp;
			}
			buffer[index++] = ch;
		}
	}

	if (!reading_from_stdin)
	{
		// loads file into memory and returns the pointer to the first character
		FILE *pFile = fopen(file_name, "r");
		printf("%s", flag);
		if (pFile == NULL)
		{
			printf("Error opening file");
		}
		file_size = getFileSize(pFile);
		printf("%ld", file_size);
		// char file_buffer[file_size + 1];
		if (file_size)
		{
			fread(file_buffer, 1, file_size, pFile);
		}
		fclose(pFile);
	}
	else
	{
		printf("Read from standard input");
		file_buffer = buffer;
		file_size = size;
	}

	// after saving file to buffer, then call the functions

	int number_of_lines = getNumberOfLines(file_buffer, file_size);
	int total_words = getNumberOfWordsInFileBuffer(file_buffer);
	if (strcmp("-c", flag) == 0)
	{
		printf("%ld", file_size);
		if (!(strcmp(file_name, "") == 0))
		{
			printf(" %s", file_name);
		}
	}
	else if (strcmp("-l", flag) == 0)
	{
		printf("\n%d", number_of_lines);
	}
	else if (strcmp("-w", flag) == 0)
	{
		printf("\n%d", total_words);
	}
	else if (strcmp(flag, "") == 0)
	{
		printf("%d %d %ld", number_of_lines, total_words, file_size);
		if (!(strcmp(file_name, "") == 0))
		{
			printf(" %s", file_name);
		}
	}
	return 0;
}

long getFileSize(FILE *pFile)
{
	fseek(pFile, 0L, SEEK_END);
	long file_size_b = ftell(pFile);
	rewind(pFile);
	return file_size_b;
}

int getNumberOfLines(char file_buffer[], long line_length)
{
	int total_lines = 0;
	for (int i = 0; i < line_length; i++)
	{
		if (file_buffer[i] == '\n')
		{
			total_lines++;
		}
	}
	return total_lines;
}

int getNumberOfWordsInFileBuffer(char file_buffer[])
{
	int inword = 0;
	int total_words = 0;
	int i = 0;
	while (file_buffer[i] != '\0')
	{
		switch (file_buffer[i])
		{
		case '\0':
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			if (inword)
			{
				inword = 0;
				total_words++;
			}
			break;
		default:
			inword = 1;
		}
		i++;
	}
	return total_words;
}