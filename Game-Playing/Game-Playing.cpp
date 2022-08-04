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

#include "Game.h"

#include "Contest.h"
#include "Noughts_and_Crosses.h"
#include "Player_FirstValidAction.h"
#include "Player_Random.h"
#include "Player_Minimax.h"
#include "Player_Alphabeta.h"

#define assertm(exp, msg) assert(((void)msg, exp))

int main()
{
    //seed random with current time
    srand((unsigned int)time(0));

    Contest c = Contest();
    auto game = Noughts_and_Crosses();
    //auto p = Player_FirstValidAction();
    auto pRandom = Player_Random();
    auto pMinmax = Player_Minimax();
    auto pAlphabeta = Player_Alphabeta();


    int n = 100;
    std::map<Game::PlayState, int> counts;
    c.PlayNGames(game, pRandom, pRandom, n, counts);
    
    //counts.clear();
    //c.PlayNGames(game, pMinmax, pRandom, n, counts);
    //
    //counts.clear();
    //c.PlayNGames(game, pRandom, pMinmax, n, counts);

    counts.clear();
    c.PlayNGames(game, pAlphabeta, pRandom, n, counts);

    counts.clear();
    c.PlayNGames(game, pRandom, pAlphabeta, n, counts);
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
