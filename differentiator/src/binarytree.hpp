#pragma once

#include <cstdlib>
#include "node.hpp"
#define ROOT(tree_ptr) tree_ptr -> Getroot()

void Delete(Node* node);
/**
 * @brief recursive tree => tex parser
 * @param node
 * @param out opened .tex file
 * @param prev_priority priority of last node
 */
void Print_Tree(Node* node, FILE* out, int prev_priority);
/**
 * @brief удаляет узел node и при помощи Delete(Node*) (из binarytree.hpp) удаляет соотв. ветку
 * и соединяет node->parent и оставш. ветку
 * @param node node ptr
 * @param side wich side would be deleted
 * @return
 */
Node* Delete_branch(Node* node, char side);

class Binarytree {
    Node* root_;
public:

    explicit Binarytree();

    explicit Binarytree(Node* root);

    ~Binarytree();

    Node* Getroot();

    void Setroot(Node* root);

    //void dump(Node* node);
};

