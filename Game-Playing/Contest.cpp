#include "Contest.h"
#include <assert.h>
#include <iostream>
#include <vector>


Game::PlayState Contest::PlayGame(Game& game, Player& p1, Player& p2)
{
    game.Reset();
    p1.SetPlayerId(1);
    p2.SetPlayerId(2);

    //while (game.GetPlayState() == Game::PlayState::Unfinished){}
    int ActionLimit = 1000;
    for (int i = 0; i < ActionLimit; i++)
    { 
        if (game.GetPlayState() != Game::PlayState::Unfinished)
        {
            return game.GetPlayState();
        }
        int Action = (game.GetActivePlayer() == 1) ? p1.ChooseAction(game) : p2.ChooseAction(game);
        game.Act(Action);
    }
    bool ActionLimitReached = true;
    assert(!ActionLimitReached);
    return game.GetPlayState();
}

void Contest::PlayNGames(Game& game, Player& p1, Player& p2, int n, std::map<Game::PlayState, int>& OutCounts)
{
    //begin count at 0
    OutCounts.emplace(Game::Tie, 0);
    OutCounts.emplace(Game::Player1Wins, 0);
    OutCounts.emplace(Game::Player2Wins, 0);

    for (int i = 0; i < n; i++)
    {
        Game::PlayState Outcome = PlayGame(game, p1, p2);
        assert(Outcome == Game::Tie || Outcome == Game::Player1Wins || Outcome == Game::Player2Wins);
        OutCounts[Outcome]++;
    }


#define NAME(o) '\'' << o.GetName() << '\''

    //print results
    std::cout << NAME(p1) << "  VS  " << NAME(p2) << '\n';
    std::cout << n << " games of " << NAME(game) << '\n';
    std::cout << OutCounts[Game::Tie] << " Ties\n";
    std::cout << OutCounts[Game::Player1Wins] << " wins for player 1\n";
    std::cout << OutCounts[Game::Player2Wins] << " wins for player 2\n" << std::endl;

#undef NAME
}
