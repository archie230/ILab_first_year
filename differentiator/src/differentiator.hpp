#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include <cstdio>
#include <cassert>
#include <cmath>

#include "constants.hpp"
#include "node.hpp"
#include "automat.hpp"
#include "binarytree.hpp"

class Differentiator {
    char* expression_;
    char* currptr_;
    std::list<Node*> node_list_;
    
    Binarytree* source_tree_;
    Binarytree* derivative_tree_;

    static std::unordered_map<char, std::function<Node*(Node*)>> calculate;
public:
    explicit Differentiator();

    ~Differentiator();

    void Create_Derivative();

private:
    void Latex();
///вызывает Read_expression() и создает дерево используя рекурсивный спуск
    void Create_source_tree();
///при помощи Differentiate() создает дерево производной
    void Create_derivative_tree();
/**
 * @brief вызывает рекурсивную функцию Simplify_tree(Node*, int&)
 * пока так не перестанет изменять дерево
 * @param tree дерево которое нужно упростить
 */
    void Simplify(Binarytree* tree);
/**
 * @brief выкидывает узлы с умножением на 1,0 и тд.
 * При каждом изменении в дереве ++change_counter;
 * @param node
 * @param change_counter
 * @return
 */
    Node* Simplify_tree(Node* node, int& change_counter);
/**
 * @brief создает дерево производной на основе source_tree_
 * @param node
 * @return
 */
    Node* Differentiate(Node* node);

    int Read_expression();

    Node* GetG();

    Node* GetN();

    Node* GetE();

    Node* GetT();

    Node* GetS();

    Node* GetP();

    Node* GetVar();
/// создает статик объект типа automat_func и использует его для нахождения имен функций
    Node* GetF();

    Node* GetId();
///создает узел и пушит его в list_node_
    Node* Create(char type, float code);

    Node* Create(char type, float code, Node* left, Node* right);
///копирует всю ветку начинающ. с node
    Node* Deep_Copy(Node* node);
///удаляет все узлы на которые есть указатели в list_node_
    void Clear();
///дамп синтаксической ошибки в терминал
    void ErrDump();
//debug dumpers
/*    void dump_list();

    void dump_source_tree();

    void dump_derivative_tree();*/
};
