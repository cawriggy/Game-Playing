#include "Contest.h"
#include "Game.h"
#include <assert.h>
#include <iostream>
#include <vector>
#include <chrono>



Game::PlayState Contest::PlayGame(Game& game, Player& p1, Player& p2)
{
    //assert(&p1 != &p2); //the players may need to be distinct (to get the correct player number)
    game.Reset();
    p1.SetPlayerId(1);
    p2.SetPlayerId(2);

    while (game.GetPlayState() == Game::PlayState::Unfinished)
    {
        int Action = (game.GetActivePlayer() == 1) ? p1.ChooseAction(game) : p2.ChooseAction(game);
        game.Do(Action);
    }
    return game.GetPlayState();
}

void Contest::PlayNGames(Game& game, Player& p1, Player& p2, int n, std::map<Game::PlayState, int>& OutCounts)
{
    for (int i = 0; i < n; i++)
    {
        Game::PlayState Outcome = PlayGame(game, p1, p2);
        assert(Outcome == Game::Tie || Outcome == Game::Player1Wins || Outcome == Game::Player2Wins);
        OutCounts[Outcome]++;
    }

    //print results
#define NAME(o) '\'' << o.GetName() << '\''
    std::cout << NAME(p1) << "  VS  " << NAME(p2) << '\n';
    std::cout << n << " games of " << NAME(game) << '\n';
    std::cout << OutCounts[Game::Tie] << " Ties\n";
    std::cout << OutCounts[Game::Player1Wins] << " wins for player 1\n";
    std::cout << OutCounts[Game::Player2Wins] << " wins for player 2\n" << std::endl;
#undef NAME
}
