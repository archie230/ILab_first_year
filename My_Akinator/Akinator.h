//
// Created by archie230
//

#pragma once
#include "str_switch.h"
#include "BinaryTree.h"

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

        cout << "//Akinator v1.0." << endl << "\t" << "author: @archie230//" << endl;
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
        tree_ -> Restore();
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

                CASE("Quit"):
                cout << "Arividerci!" << endl;
                return;


                DEFAULT:

                cout << "Enter:" << endl\
                     << "\t" << "[Help] for menu" << endl\
                     << "\t" << "[Play] to start game" << endl\
                     << "\t" << "[Save] to save your tree to the file" << endl\
                     << "\t" << "[Restore] to restore your tree from binary .txt file" << endl\
                     << "\t" << "[Clear] to clear tree" << endl\
                     << "\t" << "[Quit] to end the game" << endl;
            }
        }
    }


};