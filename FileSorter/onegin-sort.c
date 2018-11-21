//______________________________________________________________________________________________________________________
/** \brief This func reading text from file
 * @param N_symbols 
 * @param buff_sym 
 */

void read_txt(int* N_symbols, char** buff_sym)
{
    if(!buff_sym || !N_symbols)
    {
        printf("NULL pointer in function read_txt");
        return;
    }

    FILE *data = fopen(textdata, "r+");
    if (!data) {
        printf("Can't open text file!\n");
        return;
    }

    //counting number of symbols
    *N_symbols = counting_nsymbols(data);

    //filling buffer of symbols
    *buff_sym = (char *) calloc(*N_symbols + 1, sizeof(char));
    if(!*buff_sym)
    {
        printf("can't reserve memory");
    }

    fread(*buff_sym, sizeof(char), *N_symbols, data);

    *buff_sym[*N_symbols] = '\n';
    rewind(data);

    fclose(data);
}
//______________________________________________________________________________________________________________________
/**  \brief This func counting number of symbols
 * This func counting and returning number of symbols in
 * your txt file
 * @param data 
 * @return 
 */

int counting_nsymbols(FILE* data)
{
    fseek(data, 0, SEEK_END);
    int N_symbols = ftell(data);

    rewind(data);

    return N_symbols;
}
//______________________________________________________________________________________________________________________
/** \brief This func counting number of lines
 * @param N_lines 
 * @param buff_sym 
 */

void count_nlines(int* N_lines, char** buff_sym)
{
    *N_lines = 1;
    for(char* str = strchr(*buff_sym, '\n'); str != NULL; str = strchr(str, '\n'))
    {
        *str = '\0';
        while(*(++str) == '\n');
        (*N_lines)++;
    }

}
//______________________________________________________________________________________________________________________
/** \brief This func creating arrays
 * This func creating and filling array
 * of structs and original_txt
 * @param txt 
 * @param original_txt 
 * @param buff_sym 
 * @param N_lines 
 */


void create_arrays(string** txt, char*** original_txt, char** buff_sym, int* N_lines)
{
    *txt = (string *) calloc(*N_lines, sizeof(string));
    *original_txt = (char **) calloc(*N_lines, sizeof(char *));

//first line
    (*txt)->s_ptr = *buff_sym;
    (*txt)->length = strlen(*buff_sym);

    int i = 1;
    for (char *str = strchr(*buff_sym, '\0'); i < *N_lines; str = strchr(str, '\0'))
    {
        while (*(++str) == '\n');
        //if (*(str) == '\0') str++;
        (*txt + i)->length = strlen(str);
        (*txt + i)->s_ptr = str;
        *(*original_txt + i) = str;
        i++;
    }
}
//______________________________________________________________________________________________________________________
/** \brief This func coding symbols
 * This func using file txt file to convert letters to number
 * @param letter 
 * @return number of letter or special code NotALetter = -230
 */

int Ru_char_encoding(char* letter)
{
    static int counter = 0;
    static char* alph = NULL;
    static int alph_size = 0;

    if(!letter)
    {
        printf("NULL pointer in encoding");
        free(alph);
    }

    if (counter == 0)
    {
        FILE *alphabet = fopen("alphabet.txt", "r+");

        if (!alphabet) {
            printf("can't open file with alphabet");
            return -1;
        }

        fseek(alphabet, 0, SEEK_END);
        alph_size = ftell(alphabet);
        alph = (char *) calloc(alph_size + 1, sizeof(char));
        rewind(alphabet);
        fread(alph, sizeof(char), alph_size, alphabet);

        fclose(alphabet);
    }
    counter++;

    int i = 0;

    while (1)
    {
        if (alph[i] == *letter)
        {
            return alph_size - i;
        }

        if (i > alph_size) return NAL;

        i++;
    }

}
//______________________________________________________________________________________________________________________
/** \brief This func compare two strings by alphabet
 * 
 * @param str1 
 * @param str2 
 * @param char_encoding 
 * @return 
 */

int my_strcmp(char* str1, char* str2, int (*char_encoding) (char* letter))
{
    int i = 0;

    while((*str1 != '\0' ) || (*str2 != '\0'))
    {
        str1 = str1 + i;
        str2 = str2 + i;

        while(char_encoding(str1) == NAL)
            str1++;
        while(char_encoding(str2) == NAL)
            str2++;

        if(char_encoding(str1+i) > char_encoding(str2+i)) {
            return 1;
        }

        if(char_encoding(str1+i) < char_encoding(str2+i)) {
            return -1;
        }

        i++;
    }

    if(*str2 == '\0')
    {
        if (*str1 == '\0')
            return 0;
        else return 1;
    }

    if(*(str1) == '\0')
        return -1;
}
//______________________________________________________________________________________________________________________
/** \brief This func compare rwo strings by ryphm 
 * 
 * @param str1 
 * @param str2 
 * @param len1 
 * @param len2 
 * @param char_encoding 
 * @return 
 */

int my_strcmp_ryphm(char* str1, char* str2, int len1, int len2, int (*char_encoding) (char* letter))
{
    int i = 1;
    int lenght = 0;

    if (len2 > len1)
        lenght = len1;
    else lenght = len2;

    for ( i; i <= lenght; i++)
    {
        if (char_encoding( (str1 + len1 - i) ) < char_encoding( (str2 + len2 - i) )) return -1;
        if (char_encoding( (str1 + len1 - i) ) > char_encoding( (str2 + len2 - i) )) return 1;
    }
    return 0;
}
//______________________________________________________________________________________________________________________
int comp_alphabet(const void* x1, const void* x2)
{
    return my_strcmp((*(const string*) x1).s_ptr, (*(const string*) x2).s_ptr, Ru_char_encoding);
}
//______________________________________________________________________________________________________________________
int comp_ryphm(const void* x1, const void* x2)
{
    return my_strcmp_ryphm((*(const string*) x1).s_ptr, (*(const string*) x2).s_ptr, (*(const string*) x1).length, (*(const string*) x2).length, Ru_char_encoding);
}
//______________________________________________________________________________________________________________________
/** \brief This func sort and write array of strings
 * @param txt 
 * @param N_lines 
 * @param comp 
 */
void sort_andwrite(string** txt, int N_lines, int (*comp) (const void* x1, const void* x2))
{
    FILE* out = fopen(output, "a");
    if(!out)
    {
        printf("can't open output file");
        return;
    }

    if (!txt)
    {
        printf("NULL pointer in func");
        return;
    }

    qsort(*txt, N_lines, sizeof(string), comp);

    for(int i = 0; i < N_lines; i++){
        fprintf(out, "%s\n", (*(*txt+i)).s_ptr);
    }

    fprintf(out, "\n");

    fclose(out);
}
//______________________________________________________________________________________________________________________
/**
 * \brief This func write origin txt
 * @param original_txt 
 * @param N_lines 
 */
void write_originaltxt(char*** original_txt, int N_lines)
{
    FILE* out = fopen(output, "a");
    if (!out)
    {
        printf("can't open output file");
        return;
    }

    for(int i = 0; i < N_lines; i++)
    {
        fprintf(out, "%s\n", *(*(original_txt+i)));
    }

    fclose(out);
}
