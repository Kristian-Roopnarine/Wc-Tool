#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <locale.h>

/*
Wc is a tool that will return the words, byte, lines and character count of a file.
When user provides filename return words, bytes and lines.
When user provides a flag in the form of -lwc then return
*/
// size of atomic reads
int CHUNKSIZE = 16 * 1024;
bool print_words = false, print_size = false, print_lines = false, print_characters = false,
     print_filename = false;

int number_of_lines(char *);
int number_of_words(char *);
char *read_argv_params(int, char *[]);

// need to allow flags
// need to get file name from argv
// file names can't start with "-"

// need to first detect all the flags to determine the output
// input can be -wlcm or -w -l -c -m

// need to detect whether there is a file name. If there is then read the characters from that file, if not then read from stdin.

// iterate over argv
// if the first character is "-", then this is a flag
// if w or l or c or m, set appropriate flag for printing
// if first character is not hyphen, assume it is a file (can we assume it is a file?)
// from argv, if there is a file name, then set fd to filename, if not its stdin

int main(int argc, char *argv[])
{
    FILE *fd;
    setlocale(LC_ALL, "");
    size_t index = 0;
    size_t size = 0;
    int words = 0;
    int lines = 0;
    char *buffer = NULL, *tmp = NULL;
    char *file_name = (char *)malloc(sizeof(char) * CHUNKSIZE);
    file_name = read_argv_params(argc, argv);
    if (file_name)
    {
        print_filename = true;
        fd = fopen(file_name, "r");
    }
    else
    {
        fd = stdin;
    }
    int ch = EOF;
    while (ch)
    {
        ch = getc(fd);

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
    free(tmp);
    words = number_of_words(buffer);
    lines = number_of_lines(buffer);
    if (print_lines)
    {
        printf("%d", lines);
    }
    if (print_words)
    {
        printf(" %d", words);
    }
    if (print_size)
    {
        printf(" %lu", index);
    }
    if (print_filename)
    {
        printf(" %s", file_name);
    }
    fclose(fd);
    return 0;
}

int number_of_lines(char *buffer)
{
    int total_lines = 0;
    for (char c = *buffer; c; c = *++buffer)
    {
        if (c == '\n')
        {
            total_lines++;
        }
    }
    return total_lines;
}

int number_of_words(char *buffer)
{
    int total_words = 0;
    int inword = 0;
    // iterate over the pointer
    for (char c = *buffer; c; c = *++buffer)
    {
        switch (c)
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
    }
    return total_words;
}

char *read_argv_params(int argc, char *argv[])
{
    bool inflag = false;
    // iterate over the pointer
    // this will be a double for loop right?
    // build file_name from argv pointer
    for (char **pArgv = argv + 1; *pArgv != argv[argc]; pArgv++)
    {
        for (char *i = *pArgv; *i != '\0'; i++)
        {
            if (*i == '-')
            {
                inflag = true;
                continue;
            }
            if (inflag)
            {
                switch (*i)
                {
                case 'w':
                    print_words = true;
                    break;
                case 'l':
                    print_lines = true;
                    break;
                case 'c':
                    print_size = true;
                    break;
                case 'm':
                    print_characters = true;
                    break;
                default:
                    // unknown flag, maybe throw error
                    continue;
                }
            }
            else
            {
                // posiiton to file_name
                return i;
            }
        }
        inflag = false;
    }
    return 0;
}