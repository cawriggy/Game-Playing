#pragma once
#include "Player.h"
#include <vector>
#include <string>
class Player_Human :
    public Player
{
public:
    std::string GetName() { return "Player_Human"; }
    int ChooseAction(const Game& game);
};

