#pragma once
#include "Player.h"
class Player_Alphabeta :
    public Player
{
public:
    std::string GetName() { return "Alphabeta depth:" + std::to_string(GetDepthLimit()); }
    int ChooseAction(const Game& game);

    // depthLimit 0 means only consider the immediate action
    // depth n considers upto n actions after that
    int GetDepthLimit() { return depthLimit; }
    void SetDepthLimit(int limit) { depthLimit = limit; }

private:
    int depthLimit = 2;

    int ScoreTerminalState(const Game& game) const;
    int ScoreLowerBound(const Game& game) const;
    int ScoreUpperBound(const Game& game) const;
    
    int ScoreState(Game& game) const;
    void PlayoutState(Game& game) const;
    int PlayoutPolicy(const Game& game) const;


    int MinmaxState(Game& game, int depth, int a, int b);
    // int NegamaxState(Game& Game, int depth, int a, int b);
};

