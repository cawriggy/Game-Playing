#include "Player_Alphabeta_Mancala.h"
#include "Game_Mancala.h"


#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include <map>
//#include <unordered_map>
#include <stdio.h>
#include <chrono>
#include <algorithm>
#include <math.h>

#include "Game.h"

#include "Game_Mancala.h"
#include "Player_Random.h"
#include "Player_Alphabeta_Mancala.h"


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






int doMoveOrdering()
{


    //seed random with current time
    srand((unsigned int)time(0));

    
    using option = std::array<int, 6>;


    auto score = [](option moveOrder) 
    {
        //start time
        typedef std::chrono::steady_clock Clock;
        auto last = Clock::now();

        //play
        auto game = Game_Mancala();
        auto pRandom = Player_Random();
        auto abManc1 = Player_Alphabeta_Mancala();
        abManc1.SetDepthLimit(8);
        abManc1.AllActions = moveOrder;
        PlayAGame(game, pRandom, abManc1);

        //stop time
        auto time = Clock::now();
        auto diff = std::chrono::duration<double, std::milli >(time - last).count();
        return -diff; 
    };



    struct ucb1
    {
        
        struct node
        {
            double totalScore = 0;
            double numVisits = 0;
            double value(double nTrials) const
            {
                return (totalScore / numVisits) + 0.7 * (sqrt(log(nTrials) / numVisits));
            }
        };


        void display(option op)
        {
            auto a = data[op];
            for (auto i : op)
            {
                std::cout << i;
            }
            std::cout << ' ' << a.numVisits << ' ' << a.totalScore / a.numVisits << '\n';
        }


        option get() const
        {
            auto pr = std::max_element(data.begin(), data.end(), [this](const auto& a, const auto& b) {
                return a.second.value(numTrials) < b.second.value(numTrials);
            });
            assert(pr != data.end());
            return (*pr).first;
        }


        void feedback(option op, double value, double weight = 1)
        {
            numTrials += weight;
            auto& entry = data[op];
            entry.numVisits += weight;
            entry.totalScore += value;
        }

    private:
        std::map<option, node> data;
        double numTrials = 0;
    };




    auto sampler = ucb1();

    //start time
    typedef std::chrono::steady_clock Clock;
    auto alast = Clock::now();

    {//try each permutation once
        option op = { 0,1,2,3,4,5 };
        do
        {
            auto v = score(op);
            sampler.feedback(op, v);
            sampler.display(op);
        } while (std::next_permutation(op.begin(), op.end()));
    }

    //stop time
    auto atime = Clock::now();
    auto diff = std::chrono::duration<double, std::milli >(atime - alast).count();
    std::cout << diff << " ms";


    //then try samples
    for (int i = 0; i < 2000; i++)
    {
        auto op = sampler.get();
        auto v = score(op);
        sampler.feedback(op, v);
        sampler.display(op);
    }


    return 0;

}