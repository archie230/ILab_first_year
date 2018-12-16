
//______________________________________________________
//Node CTOR
Node::Node(char* data):
        data_(data),
        next_(NULL),
        prev_(NULL)
{};
//
//______________________________________________________

//______________________________________________________
//List CTOR
List::List():
        nodes_number_(0),
        head_(NULL),
        tail_(NULL)
{
    char* poison = "POISON!";

    head_ = new Node(poison);
    tail_ = new Node(poison);

    head_ -> next_ = tail_;
    tail_ -> prev_ = head_;
}
//

//List DTOR
List::~List()
{
    Node* curr = head_;
    for(int i = 0; i < nodes_number_; i++)
    {
        curr = curr -> next_;
        delete curr -> prev_;
    }

    delete head_;
    delete tail_;
}
//
//______________________________________________________

//______________________________________________________
//HashTable CTOR
HashTable::HashTable(unsigned int (*HashFunc)(const char *) ):
        num_of_cells_(HASHSIZE),
        lists_array_(NULL),
        HashFunc_(HashFunc)
{
    lists_array_ = new List* [num_of_cells_];

    for(int i = 0; i < num_of_cells_; i++)
    {
        lists_array_[i] = new List();
    }

}
//

//HashTable DTOR
HashTable::~HashTable()
{
    for(int i = 0; i < num_of_cells_; i++)
        delete lists_array_[i];

    delete lists_array_;

    printf("Arrividerci!\n");
}
//______________________________________________________

//_______________List functions_________________________

void List::FindorInsert(char* value)
{
    Node* cur = head_ -> next_;
    int counter = 0;

    while(cur != NULL)
    {
        if(strcmp(cur -> data_, value) == 0){
            counter++;
        }

        cur = cur -> next_;

    }

    if(counter > 0) {
        printf("user already registred!\n");
        return;
    }

    Node* node_ins = new Node(value);

    node_ins -> next_ = tail_;
    node_ins -> prev_ = tail_ -> prev_;

    node_ins -> next_ -> prev_ = node_ins;
    node_ins -> prev_ -> next_ = node_ins;

    nodes_number_++;

    return;
}

void List::FindandDelete(char* value)
{
    Node* cur = head_;

    while(cur != NULL)
    {
        if(strcmp(cur -> data_, value) == 0){
            cur -> prev_ -> next_ = cur -> next_;
            cur -> next_ -> prev_ = cur -> prev_;

            delete cur;
            return;
        }
        cur = cur -> next_;
    }

    printf("cant't find this user\n");

}

Node* List::Find(char* value)
{
    Node* cur = head_;

    while(cur != NULL)
    {
        if(strcmp(cur -> data_,value) == 0)
            return cur;

        cur = cur -> next_;
    }

    printf("can't find this user\n");
    return NULL;
}

void List::Clear()
{
    Node* cur_ptr = head_ -> next_;
    for(int i = 0; i < nodes_number_; i++)
    {
        cur_ptr = cur_ptr -> next_;
        delete cur_ptr -> prev_;
    }

    head_ -> next_ = tail_;
    tail_ -> prev_ = head_;

    nodes_number_ = 0;
}
//_______________________________________________________

//________________Hash functions_________________________

void HashTable::registraion(char* info)

{
    int id = HashFunc_(info);

    lists_array_[id] -> FindorInsert(info);
}

unsigned int Hash_djb2(const char* hash_param)
{
    int hash = 5381;
    int c;

    while (c = *hash_param++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % HASHSIZE;
}

unsigned int
Hash_2d (const char *hash_param)
{
    unsigned int hash = 0, i;
    unsigned int rotate = 2;
    unsigned int seed = 0x1A4E41U;
    unsigned int len = strlen(hash_param);


    for (i = 0; i != len; i++, hash_param++)
    {

        hash += sTable[(*hash_param + i) & 255];
        hash = (hash << (32 - rotate) ) | (hash >> rotate);
        hash = (hash + i ) * seed;

    }


    return (hash + len) * seed % HASHSIZE;
}

unsigned int Hash_lettersum(const char* hash_param)
{
    unsigned int hash = 0;

    for(int i = 0; i < strlen(hash_param); i++)
        hash = hash + hash_param[i];

    return hash % HASHSIZE;
}

unsigned int Hash_4(const char *hash_param)
{
    unsigned int c, hash = FNV32_BASE;
    while ((c = (unsigned char) *hash_param++))
        hash = (hash * FNV32_PRIME) ^ c;
    return hash % HASHSIZE;
}

//_______________Random number generator_________________

Ran_Num* RandomNumber_array(unsigned int size)
{
    Ran_Num* numbers = new Ran_Num;

    numbers -> Ran_array = new char*[size];
    numbers -> size = size;

    srand(time(NULL));

    for(int i = 0; i < size; i++)
    {
        numbers -> Ran_array[i] = new char[13];
        numbers -> Ran_array[i][0] = '+';
        numbers -> Ran_array[i][1] = '7';
        numbers -> Ran_array[i][2] = '9';

        for(int j = 3; j < 13; j++)
        {
            if(j == 12)
            {
                numbers -> Ran_array[i][j] = '\0';
                break;
            }

            int a = (rand() % 10);
            numbers -> Ran_array[i][j] = a + '0';
        }
    }

    return numbers;
}

void delete_RanNum_array(Ran_Num* numbers)
{
    for(int i = 0; i < numbers -> size; i++)
    {
        delete numbers -> Ran_array[i];
    }

    delete numbers -> Ran_array;
    delete numbers;
}

int create_dgrm_calculate_quality(HashTable* table, int number_of_users)
{
    FILE* hashinfo = fopen("hashinfo.csv", "w+");

    if(!hashinfo){
        printf("can't open hashinfo!\n");
        return ERR_file;
    }

    int quality = 0;
    int empty = 0;

    for(int j = 0;j < HASHSIZE; j++)
    {
        fprintf(hashinfo, "%d,", table -> lists_array_[j] -> nodes_number_);

        if((table -> lists_array_[j] -> nodes_number_) != 0) {
            quality = quality + ((table->lists_array_[j]->nodes_number_) -
                                 (HASHSIZE % number_of_users)) *
                                ((table->lists_array_[j]->nodes_number_) - (HASHSIZE % number_of_users));
        }

        if((table -> lists_array_[j] -> nodes_number_) == 0)
        {
            empty++;
        }

    }
    fclose(hashinfo);

    return (quality % (HASHSIZE-empty));
}


