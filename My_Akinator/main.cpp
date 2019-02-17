//
// Created by archie230
//

#include "Akinator.h"

int main()
{
    setlocale(LC_ALL, "Rus");

    GameManager* gm = new GameManager;

    gm -> doAkinator();

    delete gm;

    return 0;
}
