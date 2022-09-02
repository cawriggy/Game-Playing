// Game-Playing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include <map>
#include <stdio.h>
#include <chrono>

#include "Game.h"

#include "Contest.h"

#include "Noughts_and_Crosses.h"
#include "Game_Connect4.h"
#include "Game_Mancala.h"

#include "Player_FirstValidAction.h"
#include "Player_Random.h"
#include "Player_Minimax.h"
#include "Player_MinmaxWMem.h"
#include "Player_Alphabeta.h"
#include "Player_BestNodeSearch.h"
#include "Player_Alphabeta_Mancala.h"
#include "Player_Human.h"


Game::PlayState PlayAGame(auto& game, auto& p1, auto& p2)
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

void PlayNGames(auto& game, auto& p1, auto& p2, int n, std::map<Game::PlayState, int>& OutCounts)
{
    for (int i = 0; i < n; i++)
    {
        Game::PlayState Outcome = PlayAGame(game, p1, p2);
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



void maxTurnsInNGames(auto& game, auto& p1, auto& p2, int n, std::map<Game::PlayState, int>& OutCounts)
{
    int maxTurns = 0;
    for (int i = 0; i < n; i++)
    {
        Game::PlayState Outcome = PlayAGame(game, p1, p2);
        assert(Outcome == Game::Tie || Outcome == Game::Player1Wins || Outcome == Game::Player2Wins);
        OutCounts[Outcome]++;

        if (game.GetTurnNumber() > maxTurns)
        {
            maxTurns = game.GetTurnNumber();
        }
    }

    //print results
#define NAME(o) '\'' << o.GetName() << '\''
    std::cout << NAME(p1) << "  VS  " << NAME(p2) << '\n';
    std::cout << n << " games of " << NAME(game) << '\n';
    std::cout << OutCounts[Game::Tie] << " Ties\n";
    std::cout << OutCounts[Game::Player1Wins] << " wins for player 1\n";
    std::cout << OutCounts[Game::Player2Wins] << " wins for player 2\n";
    std::cout << maxTurns << " max turns\n" << std::endl;
#undef NAME
}



int main()
{

    ////play against alphabeta
    ////auto game = Game_Connect4();
    ////auto game = Game_Mancala();
    //auto game = Game_Mancala_Optimised();
    //
    //auto p2 = Player_Alphabeta_Mancala();
    //p2.SetDepthLimit(16);

    //auto p1 = Player_Human();
    //game.SetPerspective(2);
    //
    //auto r = PlayAGame(game, p2, p1);

    ////display result
    //game.Display();
    //if (r == 0)
    //{
    //    std::cout << "tie";
    //}
    //else
    //{
    //    std::cout << "winner: " << r;
    //}



    //seed random with current time
    srand((unsigned int)time(0));


    auto game = Game_Mancala();


    auto pRandom = Player_Random();
    //int depth = 5;

    auto abManc1 = Player_Alphabeta_Mancala();
    abManc1.SetDepthLimit(8);

    auto abManc2 = Player_Alphabeta_Mancala();
    abManc2.SetDepthLimit(16);

    //PlayAGame(game, abManc1, abManc2);
    //game.DisplayActionSequence();

    int n = 1000;
    std::map<Game::PlayState, int> counts;

    //time the process
    typedef std::chrono::steady_clock Clock;

    for (int i = 0; i < 5; i++)
    {
        auto last = Clock::now();

        //play some games
        counts.clear();
        //PlayNGames(game, pRandom, abManc1, n, counts);
        PlayNGames(game, pRandom, pRandom, n, counts);

        auto time = Clock::now();
        auto diff = std::chrono::duration<double, std::milli >(time - last).count();
        std::cout << diff << " ms\n";
    }


    ////wait for keypress before closing (to keep release exe terminal open)
    //auto qq = getchar();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
