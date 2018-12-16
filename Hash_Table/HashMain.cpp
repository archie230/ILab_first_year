#include <HashTableheader.h>

int main()
{
    //========CREATING ARRAY OF HASH FUNCTIONS=======
    my_type Hash_Array[3] = {};

    Hash_Array[0] = &Hash_djb2;
    Hash_Array[1] = &Hash_2d;
    Hash_Array[2] = &Hash_lettersum;
    Hash_Array[3] = &Hash_4;
    //================================================

    HashTable* table = new HashTable(Hash_Array[3]);

    Ran_Num* phoneslibrary = RandomNumber_array(1000);

    //================================================

    for(int i = 0; i < phoneslibrary -> size; i++)
    {
        table -> registraion(phoneslibrary -> Ran_array[i]);
    }

    int quality = create_dgrm_calculate_quality(table, phoneslibrary -> size);

    printf("srednekvadratichnoe otklonenie  = %d\n", quality);

    delete table;
    delete_RanNum_array(phoneslibrary);

    return 0;
}
