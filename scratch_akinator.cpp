#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>

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
public:
//___Internals___
    std::string info_;
    Node* left_;
    Node* right_;
    Node* parent_;

//___Methods_____
public:
    //___Node CTOR__
    Node(std::string info):
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

    friend BinaryTree;

};

class BinaryTree {
public:
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
    //___Tree CTOR___
    BinaryTree():
    root_(NULL),
    nodes_num_(0)
    {}

    //___Tree DTOR___
    ~BinaryTree() {
        Clear();
        nodes_num_ = POISON;
    }


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
                current -> info_ = info;

                cout << "Enter negative answer:" << endl;
                std::string info_neg;
                getline(cin, info_neg);

                cout << "Enter positive answer:" << endl;
                std::string info_pos;
                getline(cin, info_pos);

                current -> left_ = new Node(info_neg);
                current -> left_ -> parent_ = current;
                Nplus

                current -> right_ = new Node(info_pos);
                current -> right_ -> parent_ = current;
                Nplus
            }
        }
    }

    void Clear()
    {
        if (root_ != NULL) {
            Delete(root_->left_);
            Delete(root_->right_);
            delete root_;
        }

        nodes_num_ = 0;
    }

    Node* Ask(Node* node)
    {
        if(!node)
        {
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

        cout << "It's" << node -> info_ << endl;
        return node;
    }



friend GameManager;
};

class GameManager{
//___Internals___

public:
    BinaryTree* tree_;

//___Methods_____

public:
    //___GM CTOR_____
    GameManager():
    tree_(NULL)
    {
        tree_ = new BinaryTree;

        cout << "//Akinator v1.0." << endl << "\t" << "author: archie230//" << endl;
    }

    //___GM DTOR_____
    ~GameManager()
    {
        delete tree_;
    }

    void PlayGame()
    {
        Node* cur_node = tree_ -> Ask(tree_ -> root_);

        if(!cur_node)
        {
            cout << "Enter first question:" << endl;
            std::string info;
            getline(cin, info);

            tree_ -> SmartPush(info, cur_node);

            return;
        }

        else {
            cout << "Am i right?" << endl;

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
};

    int main()
    {
        setlocale(LC_ALL, "Rus");


        /*BinaryTree *main_tree = new BinaryTree;
        std::string test1, test2;

        cin >> test1;
        main_tree->SmartPush(test1, NULL);

        cout << "info for left node";
        cin >> test2;
        main_tree->SmartPush(test2, main_tree->root_->left_);

        cout << main_tree -> root_ -> info_ << "?";

        cout << "\t" <<  main_tree -> root_ -> left_ -> info_<< "?";
        cout << "\t" <<  main_tree -> root_ -> left_ -> right_ -> info_;
        cout << "\t" <<  main_tree -> root_ -> left_ -> left_ -> info_;

        cout << main_tree -> root_ -> right_ -> info_;

        delete main_tree;*/


        GameManager* gm = new GameManager;

        gm -> PlayGame();
        gm -> PlayGame();
        gm -> PlayGame();

        delete gm;
        return 0;
    }
