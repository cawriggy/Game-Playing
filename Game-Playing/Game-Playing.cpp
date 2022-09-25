// Game-Playing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <map>
#include <stdio.h>
#include <time.h>
#include <chrono>

//games
#include "Game.h"
#include "Noughts_and_Crosses.h"
#include "Game_Connect4.h"
#include "Game_Mancala.h"

//players
#include "Player_FirstValidAction.h"
#include "Player_Random.h"
#include "Player_Minimax.h"
#include "Player_MinmaxWMem.h"
#include "Player_Alphabeta.h"
#include "Player_Alphabeta_Mancala.h"
#include "Player_Human.h"


void TestConnect4Players();
void TestMancalaPlayers();
void PlayMancalaInteractively();
void PlayConnect4Interactively();
void DoMoveOrdering();


int main()
{
    TestConnect4Players();
    TestMancalaPlayers();

    //PlayMancalaInteractively();
    //PlayConnect4Interactively();

    
    ////this generates a lot of output
    //DoMoveOrdering();


    ////wait for keypress before closing (to keep release exe terminal open)
    //auto qq = getchar();

}


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

void PlayNGames(auto& game, auto& p1, auto& p2, int n, int repeats = 1)
{
    std::map<Game::PlayState, int> counts;

    //time the process
    typedef std::chrono::steady_clock Clock;

    for (int i = 0; i < repeats; i++)
    {
        auto last = Clock::now();

        //play some games
        counts.clear();
        for (int i = 0; i < n; i++)
        {
            Game::PlayState Outcome = PlayAGame(game, p1, p2);
            assert(Outcome == Game::Tie || Outcome == Game::Player1Wins || Outcome == Game::Player2Wins);
            counts[Outcome]++;
        }

        auto time = Clock::now();
        auto diff = std::chrono::duration<double, std::milli >(time - last).count();

        //print results
#define NAME(o) '\'' << o.GetName() << '\''
        std::cout << NAME(p1) << "  VS  " << NAME(p2) << '\n';
        std::cout << n << " games of " << NAME(game) << '\n';
        std::cout << counts[Game::Tie] << " Ties\n";
        std::cout << counts[Game::Player1Wins] << " wins for player 1\n";
        std::cout << counts[Game::Player2Wins] << " wins for player 2\n";
        std::cout << diff << " ms\n";
        std::cout << std::endl;
#undef NAME
    
    }
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



//play Mancala using text-based interface
void PlayMancalaInteractively() 
{
    auto game = Game_Mancala();
    
    //play against alphabeta
    auto p1 = Player_Alphabeta_Mancala();
    p1.SetDepthLimit(16);

    //you are player 2
    auto p2 = Player_Human();
    game.SetPerspective(2);
    
    auto r = PlayAGame(game, p1, p2);

    //display result
    game.Display();
    if (r == 0)
    {
        std::cout << "tie";
    }
    else
    {
        if ((r == 1) == (p2.GetName() == "Player_Human"))
        {
            std::cout << "you lost\n" << std::endl;
        }
        else
        {
            std::cout << "You Won!\n" << std::endl;
        }
    }
}


//play Connect4 using text-based interface
void PlayConnect4Interactively()
{
    auto game = Game_Connect4();

    //play against alphabeta
    auto p1 = Player_Alphabeta();
    p1.SetDepthLimit(5);

    //you are player 2
    auto p2 = Player_Human();

    auto r = PlayAGame(game, p1, p2);

    //display result
    game.Display();
    if (r == 0)
    {
        std::cout << "tie";
    }
    else
    {
        if ((r == 1) == (p2.GetName() == "Player_Human"))
        {
            std::cout << "you lost\n" << std::endl;
        }
        else
        {
            std::cout << "You Won!\n" << std::endl;
        }
    }
}


//summarise results of games between ai players for Connect4
void TestConnect4Players()
{
    //seed random with current time
    srand((unsigned int)time(0));

    auto game = Game_Connect4();
    auto pRandom = Player_Random();

    auto pAbeta1 = Player_Alphabeta();
    pAbeta1.SetDepthLimit(2);

    auto pAbeta2 = Player_Alphabeta();
    pAbeta2.SetDepthLimit(8);

    //PlayAGame(game, pAbeta1, pAbeta2);
    //game.DisplayActionSequence();

    int n = 100;

    PlayNGames(game, pRandom, pRandom, n);

    PlayNGames(game, pAbeta1, pRandom, n);
    PlayNGames(game, pRandom, pAbeta1, n);

    //deterministic so any repeats are identical
    PlayNGames(game, pAbeta1, pAbeta2, n = 1);
    PlayNGames(game, pAbeta2, pAbeta1, n = 1);

}


//summarise results of games between ai players for Mancala
void TestMancalaPlayers()
{
    //seed random with current time
    srand((unsigned int)time(0));

    auto game = Game_Mancala();
    auto pRandom = Player_Random();

    auto abManc1 = Player_Alphabeta_Mancala();
    abManc1.SetDepthLimit(2);

    auto abManc2 = Player_Alphabeta_Mancala();
    abManc2.SetDepthLimit(8);

    //PlayAGame(game, abManc1, abManc2);
    //game.DisplayActionSequence();

    int n = 100;

    PlayNGames(game, pRandom, pRandom, n);
    
    PlayNGames(game, abManc1, pRandom, n);
    //PlayNGames(game, abManc2, pRandom, n);
    PlayNGames(game, pRandom, abManc1, n);
    //PlayNGames(game, pRandom, abManc2, n);
    
    //deterministic so any repeats would be identical
    PlayNGames(game, abManc1, abManc2, n=1);
    PlayNGames(game, abManc2, abManc1, n=1);

}