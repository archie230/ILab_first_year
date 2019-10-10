#pragma once

#include <string>
#include <cstdlib>
#include <cassert>
#include "str_switch.hpp"
#include "constants.hpp"

class Func_Automat;
///
class Automat_Node {
    Automat_Node* parent_;
    Automat_Node** childs_;
    char letter_;
    int array_size_;

public:
    explicit Automat_Node(Automat_Node* parent, char letter, int array_size);

    explicit Automat_Node();

    ~Automat_Node();

    Automat_Node* operator[] (char letter);

    friend Func_Automat;
};

class Func_Automat {
    Automat_Node* head_;
public:
    explicit Func_Automat();

    ~Func_Automat();
///проходится по графу и возвращает код функции или -1
    float Get_funcid(char*& currptr);
private:
    void Delete(Automat_Node* node);
///создает граф из функций
    Automat_Node* create_graph();
    
    const int max_len = 6;
};
