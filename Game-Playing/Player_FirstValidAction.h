#pragma once
#include "Player.h"
#include "Game.h"

class Player_FirstValidAction :
    public Player
{
public:
    virtual std::string GetName();
    virtual int ChooseAction(const Game& game);
};
