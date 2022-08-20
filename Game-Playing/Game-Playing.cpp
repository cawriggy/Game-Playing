// Game-Playing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <time.h>
#include <map>
#include <stdio.h>
#include <chrono>

#include "Game.h"

#include "Contest.h"

#include "Noughts_and_Crosses.h"
#include "Game_Connect4.h"
#include "Game_Connect4_Bitboards.h"

#include "Player_FirstValidAction.h"
#include "Player_Random.h"
#include "Player_Minimax.h"
#include "Player_MinmaxWMem.h"
#include "Player_Alphabeta.h"
#include "Player_BestNodeSearch.h"
#include "Player_Alphabeta_Mancala.h"

//#define assertm(exp, msg) assert(((void)msg, exp))



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

void PlayNAGames(auto& game, auto& p1, auto& p2, int n, std::map<Game::PlayState, int>& OutCounts)
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



int main()
{
    //auto vwr = Viewer_Connect4();
    //vwr.Do(1);


    //play against alphabeta
    auto game = Game_Connect4_Bitboards();
    //auto game = Noughts_and_Crosses();
    auto p1 = Player_Alphabeta();
    p1.SetDepthLimit(8);
    auto p2 = Player_Human();
    auto c = Contest();
    auto r = c.PlayGame(game, p1, p2);

    //display result
    game.Display();
    if (r == 0)
    {
        std::cout << "tie";
    }
    else
    {
        std::cout << "winner: " << r;
    }



    ////seed random with current time
    //srand((unsigned int)time(0));

    //Contest c = Contest();
    //////auto game = Noughts_and_Crosses();
    //////auto game = Game_Connect4();
    //auto game = Game_Connect4_Bitboards();
    //////auto p = Player_FirstValidAction();
    //auto pRandom = Player_Random();
    //
    //int depth = 5;
    //
    //auto pMinmax = Player_Minimax();
    //pMinmax.SetDepthLimit(depth);

    //auto pMinmaxWmem = Player_MinmaxWMem();
    //pMinmaxWmem.SetDepthLimit(depth);

    //auto pAlphabeta = Player_Alphabeta();
    //pAlphabeta.SetDepthLimit(depth);

    //auto pAlphabeta2 = Player_Alphabeta();
    //pAlphabeta2.SetDepthLimit(depth);

    //auto pBestNode = Player_BestNodeSearch();
    //pBestNode.SetDepthLimit(depth);

    auto abManc = Player_Alphabeta_Mancala();
    abManc.SetDepthLimit(depth);

    auto abManc2 = Player_Alphabeta_Mancala();
    abManc2.SetDepthLimit(1);


    //auto p = &pBestNode;
    //auto p = &pAlphabeta;
    //auto p = &pMinmax;
    //auto p = &pMinmaxWmem;
    //auto p = &pRandom;

    //int n = 1000;
    //std::map<Game::PlayState, int> counts;

    //time the process
    typedef std::chrono::steady_clock Clock;
    //for (int i = 0; i < 5; i++)
    //{
    //    auto last = Clock::now();

    //    //play some games
    //    counts.clear();
    //    c.PlayNGames(game, pRandom, *p, n, counts);
    //    //c.PlayNGames(game, pRandom, pRandom, n, counts);

    //    auto time = Clock::now();
    //    auto diff = std::chrono::duration<double, std::milli >(time - last).count();
    //    std::cout << diff << " ms\n";
    //}


    for (int i = 0; i < 5; i++)
    {
        auto last = Clock::now();

        //play some games
        counts.clear();
        //PlayNAGames(game, abManc2, abManc, n, counts);
        //PlayNAGames(game, pRandom, abManc, n, counts);
        //PlayNAGames(game, abManc, abManc, n, counts);
        PlayNAGames(game, abManc, pRandom, n, counts);

        //c.PlayNGames(game, pRandom, pRandom, n, counts);

    //    //counts.clear();
    //    //c.PlayNGames(game, pAlphabeta2, *p, n, counts);

    //    auto time = Clock::now();
    //    auto diff = std::chrono::duration<double, std::milli >(time - last).count();
    //    std::cout << diff << " ms\n";
    //}


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
