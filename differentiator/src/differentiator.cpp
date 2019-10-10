#include "differentiator.hpp"

#define massert(expr) if(!(expr)) {\
                                    Clear();\
                                    ErrDump();\
                                    exit(0); }

std::unordered_map<char, std::function<Node*(Node*)>> Differentiator::calculate = {

    {'*', [](Node* node){
        Node* save_node = LEFT(node);
        LEFT(node) -> Set_code(LEFT(node) -> Get_code() * RIGHT(node) -> Get_code());
        Delete_branch(node, 'R');
        return save_node;}
    },
    {'+', [](Node* node){
        Node* save_node = LEFT(node);
        LEFT(node) -> Set_code(LEFT(node) -> Get_code() + RIGHT(node) -> Get_code());
        Delete_branch(node, 'R');
        return save_node;}
    },
    {'-', [](Node* node){
        Node* save_node = LEFT(node);
        LEFT(node) -> Set_code(LEFT(node) -> Get_code() - RIGHT(node) -> Get_code());
        Delete_branch(node, 'R');
        return save_node;}
    },
    {'/', [](Node* node){
        Node* save_node = LEFT(node);
        LEFT(node) -> Set_code(LEFT(node) -> Get_code() / RIGHT(node) -> Get_code());
        Delete_branch(node, 'R');
        return save_node;}
    },
    {'^', [](Node* node){
        Node* save_node = LEFT(node);
        LEFT(node) -> Set_code(powf(LEFT(node) -> Get_code(), RIGHT(node) -> Get_code()));
        Delete_branch(node, 'R');
        return save_node;}
    }

};

Differentiator::Differentiator()
        :
        expression_(nullptr),
        currptr_(nullptr),
        node_list_(),
        source_tree_(nullptr),
        derivative_tree_(nullptr)
{}

Differentiator::~Differentiator() {
    delete [] expression_;
    expression_ = nullptr;
    currptr_ = nullptr;

    delete source_tree_;
    delete derivative_tree_;

    source_tree_ = nullptr;
    derivative_tree_ = nullptr;
}

void Differentiator::Create_Derivative() {
    Create_source_tree();
    Simplify(source_tree_);
    Create_derivative_tree();

    //dump_source_tree();
    //dump_derivative_tree();
    Simplify(derivative_tree_);

    //printf("##SIMPLIFIED##\n");
    //dump_derivative_tree();
    Latex();
}

void Differentiator::Latex() {
    FILE* out = fopen("output.tex", "w");
    if(!out) {
        printf("\nCAN'T OPEN FILE output.tex in function: %s\nline: %d\n", __PRETTY_FUNCTION__, __LINE__ - 2);
        return;
    }

    if((!derivative_tree_) || (!source_tree_))
        return;

    fprintf(out, "\\documentclass{article}\n");
    fprintf(out, "\\usepackage[english,russian]{babel}\n");
    fprintf(out, "\\usepackage{amsmath,amsfonts,amssymb,amsthm,mathtools}\n\n");

    fprintf(out, "\\begin{document}\n");

    fprintf(out, "Let's take derivative of\n\n");
    fprintf(out, "$f(x) = ");
    Print_Tree(ROOT(source_tree_), out, 0);
    fprintf(out, "$\n\n");

    fprintf(out, "Derivative:\n\n");
    fprintf(out, "$\\frac{df}{dx} = ");
    Print_Tree(ROOT(derivative_tree_), out, 0);
    fprintf(out, "$\n\\end{document}");

    fclose(out);
}

void Differentiator::Create_source_tree() {
    if(Read_expression() == ERRCODE)
        return;

    source_tree_ = new(std::nothrow) Binarytree(GetG());
}

void Differentiator::Create_derivative_tree() {
    if(!source_tree_)
        return;

    derivative_tree_ = new(std::nothrow) Binarytree(Differentiate(ROOT(source_tree_)));
}
void Differentiator::Simplify(Binarytree* tree) {
    if(!tree) {
        printf("nullptr tree in %s !\n", __PRETTY_FUNCTION__);
        return;
    }

    if(!ROOT(tree))
        return;

    int change_counter = 0;
    Node* tree_root = ROOT(tree);

    do {
        change_counter = 0;
        tree_root = Simplify_tree(tree_root, change_counter);
    }
    while(change_counter > 0);
    assert(change_counter == 0);

    tree -> Setroot(tree_root);
}
Node* Differentiator::Simplify_tree(Node* node, int& change_counter) {
    if(!node)
        return nullptr;

    Node* save_node = nullptr;

    switch(node -> Get_type()) {

        case OPERATOR:
            switch(static_cast<int>(node -> Get_code())) {
                case '*':
                    if((LEFT(node) -> Get_type() == NUM) && ((LEFT(node) -> Get_code() == 1)\
                                                                || (LEFT(node) -> Get_code() == 0)))  {
                        if(LEFT(node) -> Get_code() == 1) {
                            save_node = RIGHT(node);
                            Delete_branch(node, 'L');
                            ++change_counter;
                            return Simplify_tree(save_node, change_counter);
                        }

                        else {
                            ++change_counter;
                            return Delete_branch(node, 'R');
                        }
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && ((RIGHT(node) -> Get_code() == 1)\
                                                                || (RIGHT(node) -> Get_code() == 0)))  {
                        if(RIGHT(node) -> Get_code() == 1) {
                            save_node = LEFT(node);
                            Delete_branch(node, 'R');
                            ++change_counter;
                            return Simplify_tree(save_node, change_counter);
                        }

                        else {
                            ++change_counter;
                            return Delete_branch(node, 'L');
                        }
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_type() == NUM)) {
                        ++change_counter;
                        return calculate['*'](node);
                    }

                    add_left(node, Simplify_tree(LEFT(node), change_counter));
                    add_right(node, Simplify_tree(RIGHT(node), change_counter));
                    return node;

                case '+':
                    if((LEFT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_code() == 0)) {
                        save_node = RIGHT(node);
                        Delete_branch(node, 'L');
                        ++change_counter;
                        return Simplify_tree(save_node, change_counter);
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (RIGHT(node) -> Get_code() == 0)) {
                        save_node = LEFT(node);
                        Delete_branch(node, 'R');
                        ++change_counter;
                        return Simplify_tree(save_node, change_counter);
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_type() == NUM)) {
                        ++change_counter;
                        return calculate['+'](node);
                    }

                    add_left(node, Simplify_tree(LEFT(node), change_counter));
                    add_right(node, Simplify_tree(RIGHT(node), change_counter));
                    return node;

                case '-':
                    if((LEFT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_code() == 0)) {
                        LEFT(node) -> Set_code(-1);
                        node -> Set_code('*');
                        ++change_counter;
                        add_right(node, Simplify_tree(RIGHT(node), change_counter));
                        return node;
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (RIGHT(node) -> Get_code() == 0)) {
                        save_node = LEFT(node);
                        Delete_branch(node, 'R');
                        ++change_counter;
                        return Simplify_tree(save_node, change_counter);
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_type() == NUM)) {
                        ++change_counter;
                        return calculate['-'](node);
                    }

                    add_left(node, Simplify_tree(LEFT(node), change_counter));
                    add_right(node, Simplify_tree(RIGHT(node), change_counter));
                    return node;


                case '/':
                    if((LEFT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_code() == 0)) {
                        ++change_counter;
                        return Delete_branch(node, 'R');
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && ((RIGHT(node) -> Get_code() == 1)\
                                                                || (RIGHT(node) -> Get_code() == 0))) {
                        if(RIGHT(node) -> Get_code() == 1) {
                            save_node = LEFT(node);
                            Delete_branch(node, 'R');
                            ++change_counter;
                            return Simplify_tree(save_node, change_counter);
                        }

                        if(RIGHT(node) -> Get_code() == 0) {
                            save_node = RIGHT(node);
                            Delete_branch(node, 'L');
                            save_node -> Set_code(std::numeric_limits<float>::infinity());
                            ++change_counter;
                            return save_node;
                        }
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_type() == NUM)) {
                        ++change_counter;
                        return calculate['/'](node);
                    }

                    add_left(node, Simplify_tree(LEFT(node), change_counter));
                    add_right(node, Simplify_tree(RIGHT(node), change_counter));
                    return node;

                case '^':
                    if((RIGHT(node) -> Get_type() == NUM) && (RIGHT(node) -> Get_code() == 1)) {
                        save_node = LEFT(node);
                        Delete_branch(node, 'R');
                        ++change_counter;
                        return Simplify_tree(save_node, change_counter);
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (RIGHT(node) -> Get_code() == 0)) {
                        save_node = LEFT(node);
                        Delete_branch(node, 'R');
                        save_node -> Set_code(1);
                        ++change_counter;
                        return save_node;
                    }

                    if((LEFT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_code() == 0)) {
                        ++change_counter;
                        return Delete_branch(node, 'R');
                    }

                    if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) -> Get_type() == NUM)) {
                        ++change_counter;
                        return calculate['^'](node);
                    }

                    add_left(node, Simplify_tree(LEFT(node), change_counter));
                    add_right(node, Simplify_tree(RIGHT(node), change_counter));
                    return node;

                default:
                    printf("Invalid node code in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                    node -> dump();
                    exit(0);
            }

        case FUNCTION:
            add_left(node, Simplify_tree(LEFT(node), change_counter));
            add_right(node, Simplify_tree(RIGHT(node), change_counter));
            return node;

        case VAR:
        case NUM:
            return node;


        default:
            printf("Invalid node type in %s!: \n Node:\n", __PRETTY_FUNCTION__);
            node -> dump();
            delete source_tree_;
            delete derivative_tree_;
            exit(0);
    }
}
#define PLUS(left, right) Create(OPERATOR, '+', left, right)
#define MINUS(left, right) Create(OPERATOR, '-', left, right)
#define DIVIDE(numerator, denominator) Create(OPERATOR, '/', numerator, denominator)
#define MULTI(left, right) Create(OPERATOR, '*', left, right)
#define POW(base, degree) Create(OPERATOR, '^', base, degree)
Node* Differentiator::Differentiate(Node* node){
    if(!node)
        return nullptr;

    switch(node->Get_type()) {
        case OPERATOR:
            switch(static_cast<int>(node -> Get_code())) {
                case '+':
                    return PLUS(Differentiate(LEFT(node)), Differentiate(RIGHT(node)));

                case '-':
                    return MINUS(Differentiate(LEFT(node)), Differentiate(RIGHT(node)));

                case '*':
                    return PLUS(MULTI(Differentiate(LEFT(node)), Deep_Copy(RIGHT(node))), MULTI(Deep_Copy(LEFT(node)), Differentiate(RIGHT(node))));

                case '/':
                    return DIVIDE(MINUS(MULTI(Differentiate(LEFT(node)), Deep_Copy(RIGHT(node))),MULTI(Deep_Copy(LEFT(node)),\
                                                            Differentiate(RIGHT(node)))), POW(Deep_Copy(RIGHT(node)), Create(NUM, 2)));

                case '^':
                    switch(LEFT(node) -> Get_type()) {
                        case NUM:
                            switch(RIGHT(node) -> Get_type()) {
                                case NUM:
                                    return POW(Deep_Copy(LEFT(node)), Deep_Copy(RIGHT(node)));

                                case VAR:
                                    return MULTI(Deep_Copy(node), Create(FUNCTION, LN, nullptr, Deep_Copy(LEFT(node))));

                                case FUNCTION:
                                case OPERATOR:
                                    return MULTI(Differentiate(RIGHT(node)),
                                                 MULTI(Deep_Copy(node),
                                                       Create(FUNCTION, LN, nullptr, Deep_Copy(LEFT(node)))));

                                default:
                                    printf("Invalid node type in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                                    RIGHT(node)->dump();
                                    Clear();
                                    exit(0);
                            }

                        case VAR:
                        case FUNCTION:
                        case OPERATOR:
                            switch(RIGHT(node) -> Get_type()){
                                case NUM:
                                    return MULTI(Differentiate(LEFT(node)) , MULTI(Deep_Copy(RIGHT(node)), POW(Deep_Copy(LEFT(node)), Create(NUM, RIGHT(node) -> Get_code() - 1))));

                                case VAR:
                                case FUNCTION:
                                case OPERATOR:
                                    return MULTI(Deep_Copy(node), PLUS(Create(FUNCTION, LN, nullptr, Deep_Copy(LEFT(node))), MULTI(Deep_Copy(RIGHT(node)),\
                                                DIVIDE(Differentiate(LEFT(node)), Deep_Copy(LEFT(node))))));


                                default:
                                    printf("Invalid node type in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                                    RIGHT(node) -> dump();
                                    Clear();
                                    exit(0);
                            }

                    }

                default:
                    printf("Invalid node code in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                    node -> dump();
                    Clear();
                    exit(0);
            }
        case NUM:
            return Create(NUM, 0);

        case VAR:
            return Create(NUM, 1);

        case FUNCTION:
            if((RIGHT(node) -> Get_type() == NUM) && (LEFT(node) == nullptr || LEFT(node) -> Get_type() == NUM))
                return Create(NUM, 0);
            else {
                switch (static_cast<int>(node->Get_code())) {
#define DEF_FUNC(name, code, commands, tex) case name:\
                                            commands\
                                            break;

#include "DEF_FUNC.hpp"

                    default:
                        printf("Invalid node code in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                        node->dump();
                        Clear();
                        exit(0);
#undef DEF_FUNC
                }
            }

        default:
            printf("Invalid node type in %s!: \n Node:\n", __PRETTY_FUNCTION__);
            node -> dump();
            Clear();
            exit(0);
    }
}
#undef PLUS
#undef MINUS

#undef DIVIDE

#undef MULTI

#undef POW

int Differentiator::Read_expression() {
    FILE* file = fopen("mathexpression.txt", "r");
    if(!file) {
        printf("\nCAN'T OPEN FILE mathexpression.txt in function: %s\nline: %d\n", __PRETTY_FUNCTION__, __LINE__ - 2);
        return ERRCODE;
    }

    fseek(file, 0, SEEK_END);

    int expr_size = static_cast<int>(ftell(file));
    expression_ = new(std::nothrow) char[expr_size + 1];
    expression_[expr_size] = '\0';
    rewind(file);

    fread(expression_, expr_size, sizeof(char), file);
    fclose(file);

    return 0;
}

Node* Differentiator::GetG() {
    if(!expression_) {
        printf("NULLPTR in %p -> expression_ in %s\n", this, __PRETTY_FUNCTION__);
        return nullptr;
    }

    currptr_ = expression_;
    Node* root = GetE();

    massert(*currptr_ == '\0')

    return root;
}

Node* Differentiator::GetN() {
    float val = 0;
    char* savP = currptr_;
    bool dot_flag = false;
    float mantiss_mult = 1;

    while(((*currptr_ >= '0') && (*currptr_ <= '9')) || *currptr_ == '.') {
        if(*currptr_ == '.') {
            if(dot_flag)
                massert(false)

            dot_flag = true;
            ++currptr_;
            continue;
        }

        if(!dot_flag) {
            val = val*10 + static_cast<float>(*currptr_ - '0');
        }

        else {
            val += (static_cast<float>(*currptr_ - '0') * 0.1f * mantiss_mult);
            mantiss_mult *= 0.1;
        }

        ++currptr_;
    }

    massert(currptr_ > savP)
    Node* node = Create(NUM, val);

    return node;
}

Node* Differentiator::GetE() {
    Node* head = GetT();
    Node* node1 = nullptr;
    Node* node2 = nullptr;
    char operator_ = '\0';

    while((*currptr_ == '+') || (*currptr_ == '-')) {
        operator_ = *currptr_;
        ++currptr_;

        node2 = GetT();

        node1 = head;
        head = Create(OPERATOR, operator_);

        add_left(head, node1);
        add_right(head, node2);
    }

    return head;
}

Node* Differentiator::GetT() {
    Node* head = GetS();
    Node* node1 = nullptr;
    Node* node2 = nullptr;
    char operator_ = '\0';

    while((*currptr_ == '*') || (*currptr_ == '/')) {
        operator_ = *currptr_;
        ++currptr_;

        node2 = GetS();

        node1 = head;
        head = Create(OPERATOR, operator_);

        add_left(head, node1);
        add_right(head, node2);
    }

    return head;
}

Node* Differentiator::GetS() {
    Node* head = GetP();
    Node* node1 = nullptr;
    Node* node2 = nullptr;
    char operator_ = '\0';

    while(*currptr_ == '^') {
        operator_ = *currptr_;
        ++currptr_;

        node2 = GetP();

        node1 = head;
        head = Create(OPERATOR, operator_);

        add_left(head, node1);
        add_right(head, node2);
    }

    return head;
}

Node* Differentiator::GetP() {
    if(*currptr_ == '(') {
        ++currptr_;
        Node* head = GetE();

        massert(*currptr_ == ')')

        ++currptr_;
        return head;
    }

    Node* node = GetId();

    if(!node)
        return GetN();

    return node;
}

Node* Differentiator::GetVar(){
    char var = '\0';

    if (((*currptr_ >= 'a') && (*currptr_ <= 'z')) || ((*currptr_ >= 'A') && (*currptr_ <= 'Z'))) {
        var = *currptr_;
        ++currptr_;
        return Create(VAR, var);
    }

    return nullptr;
}

Node* Differentiator::GetF() {
    static Func_Automat automat;
    float func = automat.Get_funcid(currptr_);

    if(func == static_cast<float> (ERRCODE))
        return nullptr;

    if(func == static_cast<float> (LOG)) {
        massert(*currptr_ == '(')
        ++currptr_;

        Node* base = GetE();
        massert(*currptr_ == ',')
        ++currptr_;

        Node* argument = GetE();
        massert(*currptr_ == ')')
        ++currptr_;

        Node* logariphm = Create(FUNCTION, func);

        add_left(logariphm, base);
        add_right(logariphm, argument);

        return logariphm;
    }

    else {
        Node* argument = GetP();
        Node* function = Create(FUNCTION, func);

        add_right(function, argument);

        return function;
    }
}

Node* Differentiator::GetId() {
    Node* node = GetF();

    if(!node)
        return GetVar();
    else
        return node;
}

Node* Differentiator::Create(char type, float code) {
    Node* node = new(std::nothrow) Node(type, code);

    if(!node) {
        printf("\nMEMORY ALLOC FAILED IN: %s\n", __PRETTY_FUNCTION__);
        Clear();
        exit(0);
    }

    node_list_.push_back(node);
    return node;
}

Node* Differentiator::Create(char type, float code, Node* left, Node* right) {
    Node* node = new(std::nothrow) Node(type, code, left, right, nullptr);

    if(!node) {
        printf("\nMEMORY ALLOC FAILED IN: %s\n", __PRETTY_FUNCTION__);
        Clear();
        exit(0);
    }

    if(left)
        left -> Set_parent(node);
    if(right)
        right -> Set_parent(node);

    node_list_.push_back(node);
    return node;
}

Node *Differentiator::Deep_Copy(Node *node) {
    if(!node)
        return nullptr;

    Node* copy_node = Create(node -> Get_type(), node -> Get_code(), Deep_Copy(LEFT(node)), Deep_Copy(RIGHT(node)));

    return copy_node;
}

void Differentiator::Clear() {
    if(node_list_.empty())
        return;

    auto it = node_list_.begin();
    while(it != node_list_.end()) {
        if(*it == nullptr) {
            printf("\nNULLPTR IN nodes_list_ %s\n", __PRETTY_FUNCTION__);
            continue;
        }

        delete *it;
        it++;
    }

    node_list_.clear();
}

void Differentiator::ErrDump() {
    printf("\n%s %s\n", SYNT, --currptr_);
}

/*void Differentiator::dump_list() {
    if(node_list_.empty()) {
        printf("\n empty node list(( \n");
        return;
    }

    auto it = node_list_.begin();
    while(it != node_list_.end()) {
        if(*it == nullptr) {
            printf("\nNULLPTR IN nodes_list_ %s\n", __PRETTY_FUNCTION__);
            continue;
        }

        (*it) -> dump();
        ++it;
    }
}

void Differentiator::dump_source_tree() {
    printf("\n###################################\n");
    printf("           OLD    TREE");
    printf("\n###################################\n");
    source_tree_ -> dump(ROOT(source_tree_));
}

void Differentiator::dump_derivative_tree() {
    printf("\n###################################\n");
    printf("           NEW    TREE");
    printf("\n###################################\n");
    derivative_tree_ -> dump(ROOT(derivative_tree_));

}*/

#undef massert
