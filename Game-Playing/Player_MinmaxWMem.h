#pragma once
#include "Player.h"
#include <map>
class Player_MinmaxWMem :
    public Player
{
public:
    std::string GetName() { return "minimax"; }
    int ChooseAction(const Game& game);

    int GetDepthLimit() { return depthLimit; }
    void SetDepthLimit(int limit) { depthLimit = limit; }

private:
    int depthLimit = 2;

    using state = std::vector<int>;
    using score = int;
    std::map<state, score> scoreCache;

    int ScoreTerminalState(const Game& game) const;
    int ScoreState(Game& game) const;

    int MinmaxState(Game& game, int depth);
    // int NegamaxState(Game& Game);
};

