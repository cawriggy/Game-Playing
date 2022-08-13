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

#define assertm(exp, msg) assert(((void)msg, exp))

int main()
{
    //seed random with current time
    srand((unsigned int)time(0));

    Contest c = Contest();
    //auto game = Noughts_and_Crosses();
    //auto game = Game_Connect4();
    auto game = Game_Connect4_Bitboards();
    //auto p = Player_FirstValidAction();
    auto pRandom = Player_Random();
    
    int depth = 10;
    
    auto pMinmax = Player_Minimax();
    pMinmax.SetDepthLimit(depth);

    auto pMinmaxWmem = Player_MinmaxWMem();
    pMinmaxWmem.SetDepthLimit(depth);

    auto pAlphabeta = Player_Alphabeta();
    pAlphabeta.SetDepthLimit(depth);

    auto pAlphabeta2 = Player_Alphabeta();
    pAlphabeta2.SetDepthLimit(depth);

    auto pBestNode = Player_BestNodeSearch();
    pBestNode.SetDepthLimit(depth);

    //auto p = &pBestNode;
    auto p = &pAlphabeta;
    //auto p = &pMinmax;
    //auto p = &pMinmaxWmem;
    //auto p = &pRandom;
    
    int n = 1;
    std::map<Game::PlayState, int> counts;

    //time the process
    typedef std::chrono::steady_clock Clock;
    for (int i = 0; i < 5; i++)
    {
        auto last = Clock::now();

        //play some games
        counts.clear();
        c.PlayNGames(game, pRandom, *p, n, counts);

        //counts.clear();
        //c.PlayNGames(game, pAlphabeta2, *p, n, counts);

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
