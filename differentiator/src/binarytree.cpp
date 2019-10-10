#include "binarytree.hpp"

void Delete(Node* node) {
    if (node != nullptr) {
        Delete(LEFT(node));
        Delete(RIGHT(node));
        delete node;
    }
}

void Print_Tree(Node* node, FILE* out, int prev_priority) {
    if(!out)
        return;

    if(!node)
        return;

    int curr_priority = 0;

    switch(node -> Get_type()) {
        case OPERATOR:
            curr_priority = Get_Oper_Priority(node);

            if(curr_priority < prev_priority)
                fprintf(out, "(");

            switch(static_cast<int>(node -> Get_code())) {
                case '+':
                    Print_Tree(LEFT(node), out, curr_priority);
                    fprintf(out, "+");
                    Print_Tree(RIGHT(node), out, curr_priority);
                    break;

                case '-':
                    Print_Tree(LEFT(node), out, curr_priority);
                    fprintf(out, "-");
                    Print_Tree(RIGHT(node), out, curr_priority);
                    break;

                case '*':
                    Print_Tree(LEFT(node), out, curr_priority);
                    fprintf(out, "\\cdot");
                    Print_Tree(RIGHT(node), out, curr_priority);
                    break;

                case '/':
                    fprintf(out, "\\frac{");
                    Print_Tree(LEFT(node), out, curr_priority);
                    fprintf(out, "}{");
                    Print_Tree(RIGHT(node), out, curr_priority);
                    fprintf(out, "}");
                    break;

                case '^':
                    if(LEFT(node) -> Get_type() == FUNCTION) {
                        fprintf(out, "{(");
                        Print_Tree(LEFT(node), out, curr_priority);
                        fprintf(out, ")}^{");
                    }
                    else {
                        fprintf(out, "{");
                        Print_Tree(LEFT(node), out, curr_priority);
                        fprintf(out, "}^{");
                    }
                    Print_Tree(RIGHT(node), out, curr_priority);
                    fprintf(out, "}");
                    break;

                default:
                    printf("Wrong operator code in %s\n Node:", __PRETTY_FUNCTION__);
                    node -> dump();
                    return;
            }

            if(curr_priority < prev_priority)
                fprintf(out, ")");
            return;

        case FUNCTION:
            switch(static_cast<int>(node -> Get_code())) {
#define DEF_FUNC(name, code, commands, tex) case name:\
                                                    tex

#include "DEF_FUNC.hpp"

#undef DEF_FUNC
                default:
                    printf("Wrong func code in %s\n Node:", __PRETTY_FUNCTION__);
                    node -> dump();
                    return;
            }

        case NUM:
            if(node -> Get_code() < 0)
                fprintf(out, "(%g)", node -> Get_code());
            else
                fprintf(out, "%g", node -> Get_code());
            return;

        case VAR:
            fprintf(out, "%c", static_cast<char>(node -> Get_code()));
            return;

        default:
            printf("Invalid node type in %s!: \n Node:\n", __PRETTY_FUNCTION__);
            node -> dump();
            return;
    }
}

Node* Delete_branch(Node* node, char side) {
    Node* save_node = nullptr;

    switch(side) {
        case 'L':
        case 'l':
            if(node -> Get_parent()) {
                if(LEFT(node -> Get_parent()) == node)
                    add_left(node -> Get_parent(), RIGHT(node));
                else
                    add_right(node -> Get_parent(), RIGHT(node));
            }

            else
                RIGHT(node) -> Set_parent(nullptr);

            save_node = RIGHT(node);

            Delete(LEFT(node));
            delete node;

            return save_node;

        case 'R':
        case 'r':
            if(node -> Get_parent()) {
                if (LEFT(node->Get_parent()) == node)
                    add_left(node->Get_parent(), LEFT(node));
                else
                    add_right(node->Get_parent(), LEFT(node));
            }

            else
                LEFT(node) -> Set_parent(nullptr);

            save_node = LEFT(node);

            Delete(RIGHT(node));
            delete node;

            return save_node;

        default:
            printf("Invalid side in %s", __PRETTY_FUNCTION__);
            return nullptr;
    }
}

Binarytree::Binarytree()
        :
        root_(nullptr)
{}

Binarytree::Binarytree(Node *root)
        :
        root_(root)
{}

Binarytree::~Binarytree() {
    Delete(root_);
    root_ = nullptr;
}

Node *Binarytree::Getroot() {
    return root_;
}

void Binarytree::Setroot(Node *root) {
    root_ = root;
}

/*
void Binarytree::dump(Node* node) {
    if(node != nullptr) {
        node -> dump();

        if(LEFT(node) != nullptr) {
            printf("\nLEFT:\n");
            dump(LEFT(node));
        }
        if(RIGHT(node) != nullptr) {
            printf("\nRIGHT:\n");
            dump(RIGHT(node));
        }
    }
}*/
