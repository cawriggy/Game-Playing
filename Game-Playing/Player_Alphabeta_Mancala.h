#pragma once
#include "Player.h"
//#include "Game_Mancala.h"
#include "Game_Mancala_Optimised.h"

class Player_Alphabeta_Mancala
{
public:
    using GameClass = Game_Mancala_Optimised;
    
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

private:
    int depthLimit = 2;
    int PlayerId = -1;

    int ScoreLowerBound(const GameClass& game) const;
    int ScoreUpperBound(const GameClass& game) const;
    int ScoreState(GameClass& game) const;

    int MinmaxState(GameClass& game, int depth, int a, int b);

};

