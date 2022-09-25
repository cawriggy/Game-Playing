#pragma once
#include "Player.h"
#include "Game_Mancala.h"
#include <array>

class Player_Alphabeta_Mancala
{
public:
    using GameClass = Game_Mancala;
    
    int ChooseAction(const GameClass& game);
    
    // depthLimit 0 means only consider the immediate action
    // depth n considers upto n actions after that
    int GetDepthLimit() 
    { 
        return depthLimit;
    }

    void SetDepthLimit(int limit) 
    { 
        depthLimit = limit; 
    }

    int GetPlayerId() const
    {
        return PlayerId;
    }

    void SetPlayerId(int newId)
    {
        PlayerId = newId;
    }

    std::string GetName() 
    {
        return "Alphabeta for Mancala depth:" + std::to_string(GetDepthLimit()); 
    }

    std::array<int, 6> AllActions = { 5,4,3,2,1,0 };
    //std::array<int, 6> AllActions = { 0,1,2,3,4,5 }; //this order is much slower

private:
    int depthLimit = 2;
    int PlayerId = -1;

    int ScoreLowerBound(const GameClass& game) const;
    int ScoreUpperBound(const GameClass& game) const;
    int ScoreState(GameClass& game) const;

    int MinmaxState(GameClass& game, int depth, int a, int b);

};

