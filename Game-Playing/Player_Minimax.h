#pragma once
#include "Player.h"
class Player_Minimax :
    public Player
{
public:
    std::string GetName() { return "minimax"; }
    int ChooseAction(const Game& game);

    int GetDepthLimit() { return depthLimit; }
    void SetDepthLimit(int limit) { depthLimit = limit; }

private:
    int depthLimit = 2;

    int ScoreTerminalState(const Game& game) const;
    int ScoreState(Game& game) const;
    void PlayoutState(Game& game) const;
    int PlayoutPolicy(const Game& game) const;
    
    int MinmaxState(Game& game, int depth);
    // int NegamaxState(Game& Game);
};

