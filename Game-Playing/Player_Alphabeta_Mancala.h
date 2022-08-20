#pragma once
#include "Player.h"
#include "Game_Mancala.h"
class Player_Alphabeta_Mancala //:public Player
{
public:
    using GameClass = Game_Mancala;
    std::string GetName() { return "Alphabeta for Mancala depth:" + std::to_string(GetDepthLimit()); }
    int ChooseAction(const GameClass& game);

    int GetPlayerId() const
    {
        return PlayerId;
    }

    void SetPlayerId(int newId)
    {
        PlayerId = newId;
    }

    // depthLimit 0 means only consider the immediate action
    // depth n considers upto n actions after that
    int GetDepthLimit() { return depthLimit; }
    void SetDepthLimit(int limit) { depthLimit = limit; }

private:
    int depthLimit = 2;

    int ScoreTerminalState(const GameClass& game) const;
    int ScoreLowerBound(const GameClass& game) const;
    int ScoreUpperBound(const GameClass& game) const;
    int ScoreState(GameClass& game) const;

    int MinmaxState(GameClass& game, int depth, int a, int b);
    // int NegamaxState(Game& Game, int depth, int a, int b);
    int PlayerId = -1;
};

