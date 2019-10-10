#pragma once

#include <cstdlib>
#include <cstdio>
#include "constants.hpp"

#define RIGHT(node) node->Get_right()
#define LEFT(node) node->Get_left()

class Node {
    char type_;
    float code_;

    Node* left_;
    Node* right_;
    Node* parent_;

public:
    explicit Node(char type, float code, Node* left, Node* right, Node* parent);

    explicit Node(char type, float code);

    explicit Node();

    ~Node();
//Setters
    void Set_left(Node* left);

    void Set_right(Node* right);

    void Set_type(int type);

    void Set_code(float code);

    void Set_parent(Node* parent);
//Getters
    Node* Get_left() const;

    Node* Get_right() const;

    Node* Get_parent() const;

    char Get_type() const;

    float Get_code() const;
//Debug node dumper
    void dump() const;
};

void add_left(Node* parent, Node* child);

void add_right(Node* parent, Node* child);

int Get_Oper_Priority(Node* node);
