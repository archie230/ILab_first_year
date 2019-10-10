#include "automat.hpp"

Automat_Node::Automat_Node(Automat_Node *parent, char letter, int array_size)
        :
        parent_(parent),
        childs_(nullptr),
        letter_(letter),
        array_size_(array_size)
{
    assert(array_size >= 0);

    if(array_size > 0)
        childs_ = new(std::nothrow) Automat_Node* [array_size];
    else
        childs_ = nullptr;
}

Automat_Node::Automat_Node()
        :
        parent_(nullptr),
        childs_(nullptr),
        letter_(0),
        array_size_(0)
{}

Automat_Node::~Automat_Node() {
    delete [] childs_;
    parent_ = nullptr;
    childs_ = nullptr;
    letter_ = -1;
    array_size_ = -1;
}

Automat_Node *Automat_Node::operator[](char letter) {
    for(int i = 0; i < array_size_ ; ++i) {
        if(childs_[i] -> letter_ == letter)
            return childs_[i];
    }

    return nullptr;
}

//========================================================
//======================AUTOMAT:==========================
//========================================================

Func_Automat::Func_Automat()
        :
        head_(nullptr)
{
    head_ = create_graph();
    if(!head_)
        printf("CAN'T CREATE GRAPH\n");
}

Func_Automat::~Func_Automat() {
    Delete(head_);
    head_ = nullptr;
}

void Func_Automat::Delete(Automat_Node* node) {
    if(!node)
        return;

    if(node -> childs_ != nullptr) {
        for(int i = 0; i < node -> array_size_; ++i)
            Delete(node -> childs_[i]);
    }

    delete node;
}

Automat_Node* Func_Automat::create_graph() {
    Automat_Node* head = new(std::nothrow) Automat_Node(nullptr, '\0', 6);

#define LVL1(index) head -> childs_[index]
#define LVL2(index1, index2) head -> childs_[index1] -> childs_[index2]
#define LVL3(index1, index2, index3) head -> childs_[index1] -> childs_[index2] -> childs_[index3]
#define LVL4(index1, index2, index3, index4) head -> childs_[index1] -> childs_[index2] -> childs_[index3] -> childs_[index4]
#define LVL5(index1, index2, index3, index4, index5) head -> childs_[index1] -> childs_[index2] -> childs_[index3] -> childs_[index4] -> childs_[index5]
#define LVL6(index1, index2, index3, index4, index5, index6) head -> childs_[index1] -> childs_[index2] -> childs_[index3] -> childs_[index4] -> childs_[index5] -> childs_[index6]
    try {
        //graph's 1 lvl
        LVL1(0) = new Automat_Node(head, 'a', 1);
        LVL1(1) = new Automat_Node(head, 'c', 2);
        LVL1(2) = new Automat_Node(head, 'e', 1);
        LVL1(3) = new Automat_Node(head, 'l', 2);
        LVL1(4) = new Automat_Node(head, 's', 2);
        LVL1(5) = new Automat_Node(head, 't', 2);

        //graph's 2 lvl
        LVL2(0, 0) = new Automat_Node(LVL1(0), 'r', 1);

        LVL2(1, 0) = new Automat_Node(LVL1(1), 'o', 1);
        LVL2(1, 1) = new Automat_Node(LVL1(1), 'h', 0);

        LVL2(2, 0) = new Automat_Node(LVL1(2), 'x', 1);

        LVL2(3, 0) = new Automat_Node(LVL1(3), 'n', 0);
        LVL2(3, 1) = new Automat_Node(LVL1(3), 'o', 1);

        LVL2(4, 0) = new Automat_Node(LVL1(4), 'i', 1);
        LVL2(4, 1) = new Automat_Node(LVL1(4), 'h', 0);

        LVL2(5, 0) = new Automat_Node(LVL1(5), 'g', 0);
        LVL2(5, 1) = new Automat_Node(LVL1(5), 'h', 0);

        //graph's 3 lvl
        LVL3(0, 0, 0) = new Automat_Node(LVL2(0, 0), 'c', 3);

        LVL3(1, 0, 0) = new Automat_Node(LVL2(1, 0), 's', 0);

        LVL3(2, 0, 0) = new Automat_Node(LVL2(2, 0), 'p', 0);

        LVL3(3, 1, 0) = new Automat_Node(LVL2(3, 1), 'g', 0);

        LVL3(4, 0, 0) = new Automat_Node(LVL2(4, 0), 'n', 0);

        //graph's 4 lvl
        LVL4(0, 0, 0, 0) = new Automat_Node(LVL3(0, 0, 0), 'c', 2);
        LVL4(0, 0, 0, 1) = new Automat_Node(LVL3(0, 0, 0), 's', 1);
        LVL4(0, 0, 0, 2) = new Automat_Node(LVL3(0, 0, 0), 't', 1);

        //graph's 5 lvl
        LVL5(0, 0, 0, 0, 0) = new Automat_Node(LVL4(0, 0, 0, 0), 't', 1);
        LVL5(0, 0, 0, 0, 1) = new Automat_Node(LVL4(0, 0, 0, 0), 'c', 1);

        LVL5(0, 0, 0, 1, 0) = new Automat_Node(LVL4(0, 0, 0, 1), 'i', 1);

        LVL5(0, 0, 0, 2, 0) = new Automat_Node(LVL4(0, 0, 0, 2), 'g', 0);

        //graph's 6 lvl
        LVL6(0, 0, 0, 0, 0, 0) = new Automat_Node(LVL5(0, 0, 0, 0, 0), 'g', 0);

        LVL6(0, 0, 0, 0, 1, 0) = new Automat_Node(LVL5(0, 0, 0, 0, 1), 's', 0);

        LVL6(0, 0, 0, 1, 0, 0) = new Automat_Node(LVL5(0, 0, 0, 1, 0), 'n', 0);
    }
    catch(const std::bad_alloc& exc) {
        printf("%s", exc.what());
        exit(-1);
    }
#undef LVL1
#undef LVL2
#undef LVL3
#undef LVL4
#undef LVL5
#undef LVL6

    return head;
}

float Func_Automat::Get_funcid(char*& currptr) {
    std::string func(max_len, '\0');
    func.resize(0);

    assert(head_);
    Automat_Node* currnode = head_;

    while(true) {
        if(currnode -> childs_ == nullptr) {
            SWITCH(func) {
                CASE("arcsin"):
                    return ARCSIN;

                CASE("arccos"):
                    return ARCCOS;

                CASE("arctg"):
                    return ARCTG;

                CASE("arcctg"):
                    return ARCCTG;

                CASE("cos"):
                    return COS;

                CASE("ch"):
                    return CH;

                CASE("exp"):
                    return EXP;

                CASE("ln"):
                    return LN;

                CASE("log"):
                    return LOG;

                CASE("sin"):
                    return SIN;

                CASE("sh"):
                    return SH;

                CASE("tg"):
                    return TG;

                CASE("th"):
                    return TH;
            }
        }

        currnode = (*currnode)[*currptr];

        if(!currnode) {
            return -1;
        }

        func.push_back(*currptr);
        ++currptr;
    }
}
