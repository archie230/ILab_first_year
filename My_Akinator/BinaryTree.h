//
// Created by archie230
//

#pragma once
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <cmath>

//____DEFINES____
#define Nplus nodes_num_++;
#define POISON -230

//_______________
using std::cout;
using std::cin;
using std::endl;
using std::getline;

//_______________
class BinaryTree;
class GameManager;

//___Modules_____
class Node {

//___Internals___
    std::string info_;
    Node* left_;
    Node* right_;
    Node* parent_;

//___Methods_____
public:

//----------------------------------
    //___Node CTOR__
    explicit Node(std::string info):
            info_(info),
            left_(NULL),
            right_(NULL),
            parent_(NULL)
    {}


    //___Node DTOR__
    ~Node(){
        info_ = "POISON";
        left_ = NULL;
        right_ = NULL;
        parent_ = NULL;
    }
//----------------------------------

    friend BinaryTree;
    friend GameManager;
};

class BinaryTree {

//___Internals___
    Node *root_;
    int nodes_num_;

//___Methods_____
    void Delete(Node *node_) {
        if (node_ != NULL) {
            Delete(node_->left_);
            Delete(node_->right_);
            delete node_;
        }
    }

public:
//--------------------------
    //___Tree CTOR___
    explicit BinaryTree():
            root_(NULL),
            nodes_num_(0)
    {}


    //___Tree DTOR___
    ~BinaryTree() {
        Clear();
        nodes_num_ = POISON;
    }
//--------------------------

private:
    void SmartPush(std::string info, Node* current) {

        switch (nodes_num_) {
            case 0: {
                Node *node = new Node(info);
                root_ = node;
                Nplus

                cout << "Enter negative answer:" << endl;
                std::string info_n;
                getline(cin, info_n);

                cout << "Enter positive answer:" << endl;
                std::string info_p;
                getline(cin, info_p);

                root_ -> left_ = new Node(info_n);
                Nplus
                root_ -> left_ -> parent_ = root_;

                root_ -> right_ = new Node(info_p);
                Nplus
                root_ -> right_ -> parent_ = root_;
                break;
            }

            default: {
                std::string old_info = current -> info_;

                current -> info_ = info;

                cout << "So, who is that?" << endl;
                std::string info_p;
                getline(cin, info_p);

                current -> left_ = new Node(old_info);
                current -> left_ -> parent_ = current;
                Nplus

                current -> right_ = new Node(info_p);
                current -> right_ -> parent_ = current;
                Nplus
            }
        }
    }


    void Clear()
    {
        if (root_ != NULL) {
            Delete(root_);
        }

        nodes_num_ = 0;
        root_ = NULL;
    }


    void Save(){

        cout << "Enter name of file in .txt format:" << endl;
        std::string file_name;
        getline(cin, file_name);

        FILE* tree_write = fopen(file_name.c_str(), "wb");
        Write(tree_write, root_);
        fclose(tree_write);

    }


    void Write(FILE* file, Node* node)
    {
        char open = '['; // const
        char space = ' ';
        char N = 'N';
        char close = ']';

        fwrite(&open, sizeof(char), 1, file);

        short size = node -> info_.size();

        fwrite(&size, sizeof(short), 1, file);

        fwrite(node -> info_.data(), sizeof(char), size, file);

        //__left tree__
        if(node -> left_ != NULL){
            fwrite(&space, sizeof(char), 1, file);
            Write(file, node -> left_);
        }

        else{
            fwrite(&N, sizeof(char), 1, file);
        }

        //__right tree__
        if(node -> right_ != NULL){
            fwrite(&space, sizeof(char), 1, file);
            Write(file, node -> right_);
        }

        else{
            fwrite(&N, sizeof(char), 1, file);
        }

        fwrite(&close, sizeof(char), 1, file);
    }


    void Restore()
    {
        cout << "Enter name of file in .txt format:" << endl;
        std::string file_name;
        getline(cin, file_name);

        FILE* tree_read = fopen(file_name.c_str(), "rb");

        if(!tree_read){
            cout << "Can't open " << file_name << "!" << endl;
            return;
        }

        Clear();
        root_ = Read(tree_read, root_, NULL);

        if(root_ == NULL)
            cout << "File isn't correct";

        int deep;
        Node* Bad_Node;
        Deepness(deep, Bad_Node, 1);

        nodes_num_ = (int) pow(2, deep) - 1;

        fclose(tree_read);
    }


    Node* Read(FILE* file, Node* node, Node* parent)
    {
        if(fgetc(file) == '[')
        {
            short len = 0;
            fread(&len, 1 , sizeof(short), file);

            std::string info(len, '\0');
            fread(&info[0], len, sizeof(char), file);

            node = new Node(info);
            node -> parent_ = parent;

            if(fgetc(file) == ' ')
            {
                node -> left_ = Read(file, node -> left_, node);
            }

            if(fgetc(file) == ' ')
            {
                node -> right_ = Read(file, node -> right_, node);
            }

            fgetc(file);

            return node;
        }

        else return NULL;
    }


    void Deepness(int& deep, Node*& deepleft, int mode)
    {

        switch (mode) {
            case 1: {
                deepleft = NULL;

                Node *node = root_;
                deep = 0;

                while ((node->left_ != NULL) || (node->right_ != NULL)) {
                    node = node->left_;
                    deep++;
                }
                break;
            }

            case 2:{
                deep = 0;

                Node *node = root_;
                while ((node->left_ != NULL) || (node->right_ != NULL))
                    node = node->left_;

                deepleft = node;
                break;
            }

            default:
                cout << "incorrect mode of function void Deepness()";
        }
    }


    friend GameManager;
};