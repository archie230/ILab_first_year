#include "node.hpp"

Node::Node(char type, float code, Node *left, Node *right, Node *parent)
        :
        type_(type),
        code_(code),
        left_(left),
        right_(right),
        parent_(parent)
{}

Node::Node(char type, float code)
        :
        type_(type),
        code_(code),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
{}

Node::Node()
        :
        type_(0),
        code_(),
        left_(nullptr),
        right_(nullptr),
        parent_(nullptr)
{}


Node::~Node() {
    type_ = 0;
    code_ = 0;
    left_ = nullptr;
    right_ = nullptr;
    parent_ = nullptr;
}

void Node::Set_left(Node *left) {
    left_ = left;
}

void Node::Set_right(Node *right) {
    right_ =right;
}

void Node::Set_type(int type) {
    type_ = type;
}

void Node::Set_code(float code) {
    code_ = code;
}

void Node::Set_parent(Node *parent) {
    parent_ = parent;
}

void Node::dump() const{
    printf("\n-----------\n");
    printf("type: %c\n", type_);

    if(type_ != FUNCTION) {
        if(type_ == OPERATOR || type_ == VAR)
            printf("code: %c\n", static_cast<char>(code_));
        else
            printf("code: %g\n", code_);
    }

    else {
        switch (static_cast<int>(code_)) {
#define DEF_FUNC(name, code, commands, tex) case code:\
                                        printf("code: "#name);\
                                        printf("\n");\
                                        break;

#include "DEF_FUNC.hpp"

            default:
                printf("Invalid function code in %s!: \n Node:\n", __PRETTY_FUNCTION__);
                dump();
        }
#undef DEF_FUNC
    }

    printf("this: %p\n", this);
    printf("left: %p\n", left_);
    printf("right: %p\n", right_);
    printf("parent: %p\n", parent_);
}

Node* Node::Get_left() const{
    return left_;
}

Node* Node::Get_right() const{
    return right_;
}

Node* Node::Get_parent() const{
    return parent_;
}

char Node::Get_type() const{
    return type_;
}

float Node::Get_code() const{
    return code_;
}

void add_left(Node *parent, Node *child) {
    if(parent)
        parent -> Set_left(child);
    if(child)
        child -> Set_parent(parent);
}

void add_right(Node *parent, Node *child) {
    if(parent)
        parent -> Set_right(child);
    if(child)
        child -> Set_parent(parent);
}

int Get_Oper_Priority(Node *node) {
    if(!node)
        return ERRCODE;

    if(node -> Get_type() != OPERATOR) {
        printf("Wrong Node type in %s\n Node:", __PRETTY_FUNCTION__);
        node -> dump();
        return ERRCODE;
    }

    switch (static_cast<int>(node -> Get_code())) {
        case '+':
            return add_priority;

        case '-':
            return sub_priority;

        case '*':
            return mul_priority;

        case '/':
            return div_priority;

        case '^':
            return pow_priority;

        default:
            printf("Wrong operator code in %s\n Node:", __PRETTY_FUNCTION__);
            node -> dump();
            return ERRCODE;
    }
}
