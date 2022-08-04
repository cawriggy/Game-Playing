#pragma once
#include "Player.h"
class Player_Random :
    public Player
{
public:
    std::string GetName();
    int ChooseAction(const Game& game);
};

