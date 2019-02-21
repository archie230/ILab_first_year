//
// Created by archie230
//

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include "str_switch.h"

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
    root_(NULL)
    {}


    //___Tree DTOR___
    ~BinaryTree() {
        Clear();
    }
//--------------------------

private:
    void SmartPush(std::string info, Node* current) {

        if(!current){
                Node *node = new Node(info);
                root_ = node;

                cout << "Enter negative answer:" << endl;
                std::string info_n;
                getline(cin, info_n);

                cout << "Enter positive answer:" << endl;
                std::string info_p;
                getline(cin, info_p);

                root_ -> left_ = new Node(info_n);
                root_ -> left_ -> parent_ = root_;

                root_ -> right_ = new Node(info_p);
                root_ -> right_ -> parent_ = root_;
            }

            if(current){
                std::string old_info = current -> info_;

                current -> info_ = info;

                cout << "So, who is that?" << endl;
                std::string info_p;
                getline(cin, info_p);

                current -> left_ = new Node(old_info);
                current -> left_ -> parent_ = current;

                current -> right_ = new Node(info_p);
                current -> right_ -> parent_ = current;
            }
    }


    void Clear()
    {
        if (root_ != NULL) {
            Delete(root_);
        }

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


    Node* PreSearch(std::string object, Node* node)
    {
        if( (node -> info_ == object) && (node -> left_ == NULL) && (node -> right_ == NULL) )
            return node;

        if(node -> left_ != NULL){
            Node* nodeL = PreSearch(object, node -> left_);
            if(nodeL) return nodeL;
        }

        if(node -> right_ != NULL){
            Node* nodeR = PreSearch(object, node -> right_);
            if(nodeR) return nodeR;
        }

        return NULL;
    }


    int NodeDeepness(Node* node)
    {
        int counter = 0;

        while(node -> parent_ != NULL)
        {
            node = node -> parent_;
            counter++;
        }

        return counter;
    }


    Node**  WayToNemo(Node* nemo)
    {
        if(!nemo)
            return NULL;

        Node* node = nemo;

        int counter = NodeDeepness(nemo);

        Node** way = new Node*[counter+1];

        way[counter] = nemo;

        for(int i = counter - 1; i >= 0; i--)
        {
            way[i] = nemo -> parent_;
            nemo = nemo -> parent_;
        }

        return way;
    }




friend GameManager;
};

class GameManager{
//___Internals___

    BinaryTree* tree_;

//___Methods_____

public:
//----------------------------------
    //___GM CTOR_____
    explicit GameManager():
    tree_(NULL)
    {
        tree_ = new BinaryTree;

        cout << "//Akinator v1.5." << endl << "\t" << "author: @archie230//" << endl;
    }


    //___GM DTOR_____
    ~GameManager()
    {
        delete tree_;
    }
//----------------------------------

    Node* Ask(Node* node)
    {
        if(!node) {
            return NULL;
        }

        while(((node -> left_) != NULL) & ((node -> right_ ) != NULL))
        {
            cout << node -> info_ << "?" << endl;

            std::string answer;
            getline(cin, answer);

            if(answer == "no")
                node = node -> left_;

            if(answer == "yes")
                node =  node -> right_;
        }

        return node;
    }


    void PlayGame()
    {
        Node* cur_node = Ask(tree_ -> root_);

        if(!cur_node)
        {
            cout << "Enter first question:" << endl;
            std::string info;
            getline(cin, info);

            tree_ -> SmartPush(info, cur_node);

            return;
        }

        else {

            cout <<"It's " << cur_node -> info_  << endl << "Am i right?" << endl;

            std::string answer;
            getline(cin, answer);

            if (answer == "yes")
                cout << "excellent!" << endl;

            if (answer == "no") {
                cout << "What is difference?:" << endl;

                std::string info;
                getline(cin, info);

                tree_ -> SmartPush(info, cur_node);
            }
        }

        cout << "==========" << endl;
    }


    void Restore()
    {
        if(tree_)
            tree_ -> Clear();

        tree_ -> Restore();
    }


    void Compare()
    {
        std::string object1, object2;

        cout << "Enter first object:" << endl;
        getline(cin, object1);

        cout << "Enter second object:" << endl;
        getline(cin, object2);

        Node* nemo1 = tree_ -> PreSearch(object1, tree_ -> root_);
        int deep1 = tree_ -> NodeDeepness(nemo1);

        Node* nemo2 = tree_ -> PreSearch(object2, tree_ -> root_);
        int deep2 = tree_ -> NodeDeepness(nemo2);

        if(!nemo1){
            cout << "can't find" << object1 << endl;
            return;
        }

        if(!nemo2){
            cout << "can't find" << object2 << endl;
            return;
        }

        Node** way1 = tree_ -> WayToNemo(tree_ -> PreSearch(object1, tree_ -> root_));
        Node** way2 = tree_ -> WayToNemo(tree_ -> PreSearch(object2, tree_ -> root_));

        int min_deep = std::min(deep1, deep2);

        int counter = 0;

        for(int i = 1; i < min_deep; i++)
        {
            if(way1[i] == way2[i])
                counter++;
        }

        if(counter == 0)
        {
            cout << "This is 2 absolutely different objects! So there are they descriptions" << endl;

            cout << nemo1 -> info_ << ": ";
            for(int i = 0; i < deep1; i++){

                if(way1[i] -> left_ == way1[i+1])
                    cout << "Not" << way1[i] -> info_ << " ";

                if(way1[i] -> right_ == way1[i+1])
                    cout << way1[i] -> info_ << " ";

            }

            cout << endl;

            cout << nemo2 -> info_ << ": ";
            for(int i = 0; i <= deep2; i++){


                if(way2[i] -> left_ == way2[i+1])
                    cout << "Not " << way2[i] -> info_ << " ";

                if(way2[i] -> right_ == way2[i+1])
                    cout << way2[i]->info_ << " ";
            }
        }

        else{
            cout << "They both: ";
            for(int i = 0; i < min_deep - 1; i++){

                if(way1[i] -> left_ == way1[i+1])
                    cout << "Not" << way1[i] -> info_ << " ";

                if(way1[i] -> right_ == way1[i+1])
                    cout << way1[i] -> info_ << " ";
            }

            cout << endl << "But," << endl;

            cout << nemo1 -> info_ << ": ";
            for(int i = min_deep - 1; i < deep1; i++){

                if(way1[i] -> left_ == way1[i+1])
                    cout << "Not" << way1[i] -> info_ << " ";

                if(way1[i] -> right_ == way1[i+1])
                    cout << way1[i] -> info_ << " ";
            }

            cout << endl;

            cout << nemo2 -> info_ << ": ";
            for(int i = min_deep - 1; i < deep2; i++){

                if(way2[i] -> left_ == way2[i+1])
                    cout << "Not" << way2[i] -> info_ << " ";

                if(way2[i] -> right_ == way2[i+1])
                    cout << way2[i] -> info_ << " ";
            }

        }


        cout << endl;


        delete [] way1;
        delete [] way2;
    }


    void Description()
    {
         cout << "Enter object name:" << endl;

         std::string object;
         getline(cin, object);

         Node* nemo = tree_ -> PreSearch(object, tree_ -> root_);

         if(!nemo){
             cout << "can't find" << object << endl;
             return;
         }

         Node** way = tree_ -> WayToNemo(nemo);

         int deep = tree_ -> NodeDeepness(nemo);

         cout << object << ": ";

         for(int i = 0; i < deep; i++){

                 if(way[i] -> left_ == way[i+1])
                     cout << "Not" << way[i] -> info_ << " ";

                 if(way[i] -> right_ == way[i+1])
                     cout << way[i] -> info_ << " ";

         }

         delete [] way;
     }


    void doAkinator()
    {
        cout << "Enter [Help] for menu" << endl;
        std::string command;

        while(true)
        {
            getline(cin, command);

            SWITCH (command)
            {
                CASE("Help"):

                    cout << "Enter:" << endl\
                     << "\t" << "[Help] for menu" << endl\
                     << "\t" << "[Play] to start game" << endl\
                     << "\t" << "[Save] to save your tree to the file" << endl\
                     << "\t" << "[Restore] to restore your tree from binary .txt file" << endl\
                     << "\t" << "[Descrpt] to find and descripe your character" << endl\
                     << "\t" << "[Comp] to compare to objects" << endl\
                     << "\t" << "[Clear] to clear tree" << endl\
                     << "\t" << "[Quit] to end the game" << endl;
                    break;

                CASE("Play"):
                    PlayGame();
                    break;

                CASE("Save"):
                    tree_ -> Save();
                    break;

                CASE("Restore"):
                    tree_ -> Restore();
                    break;

                CASE("Cleare"):
                    tree_ -> Clear();

                CASE("Descrpt"): {
                    Description();
                    break;
                }

                CASE("Comp"):{
                    Compare();
                    break;
                }

                CASE("Quit"):
                    cout << "Arividerci!" << endl;
                    return;


                DEFAULT:

                    cout << "Enter:" << endl\
                     << "\t" << "[Help] for menu" << endl\
                     << "\t" << "[Play] to start game" << endl\
                     << "\t" << "[Save] to save your tree to the file" << endl\
                     << "\t" << "[Restore] to restore your tree from binary .txt file" << endl\
                     << "\t" << "[Descrpt] to find and descripe your character" << endl\
                     << "\t" << "[Comp] to compare to objects" << endl\
                     << "\t" << "[Clear] to clear tree" << endl\
                     << "\t" << "[Quit] to end the game" << endl;
            }
        }
      }


};
