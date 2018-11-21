#include "onegin-sort.h"

int main()
{
    setlocale(LC_ALL, "Rus");

    int N_symbols, N_lines = 0;
    char* buff_sym = NULL;  //text buff
    char** original_txt = NULL; //saving original txt
    string* txt = NULL; //pointer for array of structs

    //read file
    read_txt(&N_symbols, &buff_sym);

    //counting number of nlines
    count_nlines(&N_lines, &buff_sym);

    printf("Number of symbols: %d Number of lines: %d\n", N_symbols, N_lines);

    //creating array of structs
    create_arrays(&txt, &original_txt, &buff_sym, &N_lines);

    sort_andwrite(&txt, N_lines, comp_alphabet);
    sort_andwrite(&txt, N_lines, comp_ryphm);
    write_originaltxt(&original_txt, N_lines);


    free(buff_sym);
    free(txt);
    free(original_txt);

    return 0;
}
