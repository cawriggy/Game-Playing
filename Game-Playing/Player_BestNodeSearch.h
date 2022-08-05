#pragma once
#include "Player.h"
class Player_BestNodeSearch :
    public Player
{
public:
    std::string GetName() { return "BestNodeSearch"; }
    int ChooseAction(const Game& game);

    // depthLimit 0 means only consider the immediate action
    // depth n considers upto n actions after that
    int GetDepthLimit() { return depthLimit; }
    void SetDepthLimit(int limit) { depthLimit = limit; }

private:
    int depthLimit = 2;
    int previousScoreTest = 0;
    bool HasDoneInitialSearch = false;

    int ScoreTerminalState(const Game& game) const;
    int ScoreLowerBound(const Game& game) const;
    int ScoreUpperBound(const Game& game) const;

    int ScoreState(Game& game) const;
    void PlayoutState(Game& game) const;
    int PlayoutPolicy(const Game& game) const;


    int MinmaxState(Game& game, int depth, int a, int b);
    // int NegamaxState(Game& Game, int depth, int a, int b);

    int NextTestScore(int a, int b, int numOptions)
    {
        return 1 + a + (b - a) / 2; //* (numOptions - 1) / numOptions;
        //return a + (b - a) * (numOptions - 1) / numOptions;
    }

    int BestNodeSearch(const Game& game, int a, int b);

};


