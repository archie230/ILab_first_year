#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#define NAL -230

const char textdata[] = "tests_read.txt";
const char output[] = "tests_write.txt";

//______________________________________________________________________________________________________________________
typedef struct strings{
    int length;
    char* s_ptr;
}string;
//______________________________________________________________________________________________________________________
void read_txt(int*, char**);

int counting_nsymbols(FILE*);

void count_nlines(int*, char**);

void create_arrays(string**, char***, char**, int*);

int Ru_char_encoding(char*);

int my_strcmp(char*, char*, int (*char_encoding) (char*));

int my_strcmp_ryphm(char*, char*, int, int, int (*char_encoding) (char*));

int comp_alphabet(const void*, const void*);

int comp_ryphm(const void*, const void*);

void sort_andwrite(string** txt, int N_lines, int (*comp) (const void* x1, const void* x2));

void write_originaltxt(char***, int);

#include "onegin-sort.c"
