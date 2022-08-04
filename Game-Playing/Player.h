#pragma once

#include <string>
#include "Game.h"

class Player
{
public:
    int GetPlayerId() const
    {
        return PlayerId; 
    }

    void SetPlayerId(int newId) 
    { 
        PlayerId = newId; 
    }

    virtual std::string GetName() = 0;
    virtual int ChooseAction(const Game& game) = 0;

private:
    int PlayerId = -1;
};