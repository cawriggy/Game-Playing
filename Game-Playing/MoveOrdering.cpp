#include "Player_Alphabeta_Mancala.h"
#include "Game_Mancala.h"

#include <random>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <time.h>
#include <map>
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


struct option
{
    std::array<int, 6> MoveOrder = { 0,1,2,3,4,5 };
    unsigned int numChosen = 0;

    void shuffleUnchosen()
    {
        if (IsFullyChosen()) { return; }

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(MoveOrder.begin() + numChosen, MoveOrder.end(), g);

        //std::copy(MoveOrder.begin(), MoveOrder.end(), std::ostream_iterator<int>(std::cout, " "));
        //std::cout << "\n";
    }


    option completed() const
    {
        auto copy = option(*this);
        copy.shuffleUnchosen();
        copy.numChosen = (unsigned)MoveOrder.size();
        return copy;
    }


    bool IsFullyChosen() const { return numChosen >= MoveOrder.size(); }

    bool IsTerminal() const { return IsFullyChosen(); }

    bool operator<(const option& other) const
    {
        if (numChosen < other.numChosen) { return true; }
        for (unsigned i = 0; i < numChosen; i++)
        {
            if (MoveOrder[i] < other.MoveOrder[i])
            {
                return true;
            }
        }
        return false;
    }

    std::vector<option> GetChildNodes()
    {
        assert(numChosen <= MoveOrder.size());

        std::vector<option> nodes;
        nodes.reserve(MoveOrder.size() - numChosen);
        for (int i = numChosen; i < MoveOrder.size(); i++)
        {
            auto copy = option(*this);
            std::swap(copy.MoveOrder[numChosen], copy.MoveOrder[i]);
            copy.numChosen++;
            nodes.push_back(copy);
        }
        return nodes;
    }
};


struct ucbt
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
        std::cout << "n: " << totalFeedback;
        std::cout << ", selection: ";
        for (auto i : op.MoveOrder)
        {
            std::cout << i;
        }

        if (data.contains(op))
        {
            auto& a = data[op];
            std::cout << ", visits: " << a.numVisits << ", mean score: " << a.totalScore / a.numVisits << '\n';
        }
    }


    option get(option root = option())
    {
        auto op = root;//option();

        while (data.contains(op))
        {
            double n = data[op].numVisits;
            auto children = op.GetChildNodes();

            //return node with no children
            if (children.size() == 0)
            {
                return op.completed();
            }


            //return an untried child
            for (const auto& child : children)
            {
                if (!data.contains(child))
                {
                    return child.completed();
                }
            }

            //select the best child
            auto pr = std::max_element(children.begin(), children.end(), [this, n](const option& a, const option& b) {
                return data[a].value(n) < data[b].value(n);
            });
            assert(pr != data.end());
            op = *pr;



            //auto& best = children[0];
            //double bestScore = data[best].value(n);

            //for (const auto& child : children)
            //{
            //    auto v = data[child].value(n);
            //    if (v > bestScore)
            //    {
            //        bestScore = v;
            //        best = child;
            //    }
            //}
            //op = best;

        }

        return op.completed();

    }


    //update scores for visited nodes and add upto 1 new node
    void feedback(option op, double value, double weight = 1)
    {
        totalFeedback += weight;
        auto copy = option(op);
        for (unsigned i = 0; i <= op.numChosen; i++)
        {
            copy.numChosen = i;
            if (data.contains(copy))
            {
                data[copy].totalScore += value;
                data[copy].numVisits += weight;
            }
            else
            {
                data[copy].totalScore += value;
                data[copy].numVisits += weight;
                //break;//add at most one new node per feedback call
            }
        }

    }

private:
    std::map<option, node> data;
    double totalFeedback = 0;
};




int doMoveOrdering()
{
    //
    // identify a good static move ordering for for alphabeta mancala
    //


    // score a move rdering by how long it takes to play against a random player
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
        abManc1.AllActions = moveOrder.MoveOrder;
        PlayAGame(game, pRandom, abManc1);

        //stop time
        auto time = Clock::now();
        auto diff = std::chrono::duration<double, std::milli >(time - last).count();
        return -diff; 
    };


    auto sampler = ucbt();

    for (int i = 0; i < 500; i++)
    {
        auto op = sampler.get();
        auto v = score(op);
        sampler.feedback(op, v);
        sampler.display(op);
    }

    return 0;

}









//int doMoveOrdering_allInOne()
//{
//
//    //
//    // try to identfy the best move to try first
//    // could try montecarlo, could try minmax
//    //
//
//
//
//    //seed random with current time
//    srand((unsigned int)time(0));
//
//
//    using option = std::array<int, 6>;
//
//
//    auto score = [](option moveOrder)
//    {
//        //start time
//        typedef std::chrono::steady_clock Clock;
//        auto last = Clock::now();
//
//        //play
//        auto game = Game_Mancala();
//        auto pRandom = Player_Random();
//        auto abManc1 = Player_Alphabeta_Mancala();
//        abManc1.SetDepthLimit(8);
//        abManc1.AllActions = moveOrder;
//        PlayAGame(game, pRandom, abManc1);
//
//        //stop time
//        auto time = Clock::now();
//        auto diff = std::chrono::duration<double, std::milli >(time - last).count();
//        return -diff;
//    };
//
//
//
//    struct ucb1
//    {
//
//        struct node
//        {
//            double totalScore = 0;
//            double numVisits = 0;
//            double value(double nTrials) const
//            {
//                return (totalScore / numVisits) + 0.7 * (sqrt(log(nTrials) / numVisits));
//            }
//        };
//
//
//        void display(option op)
//        {
//            auto a = data[op];
//            for (auto i : op)
//            {
//                std::cout << i;
//            }
//            std::cout << ' ' << a.numVisits << ' ' << a.totalScore / a.numVisits << '\n';
//        }
//
//
//        option get() const
//        {
//            auto pr = std::max_element(data.begin(), data.end(), [this](const auto& a, const auto& b) {
//                return a.second.value(numTrials) < b.second.value(numTrials);
//            });
//            assert(pr != data.end());
//            return (*pr).first;
//        }
//
//
//        void feedback(option op, double value, double weight = 1)
//        {
//            numTrials += weight;
//            auto& entry = data[op];
//            entry.numVisits += weight;
//            entry.totalScore += value;
//        }
//
//    private:
//        std::map<option, node> data;
//        double numTrials = 0;
//    };
//
//
//
//
//    auto sampler = ucb1();
//
//    //start time
//    typedef std::chrono::steady_clock Clock;
//    auto alast = Clock::now();
//
//    {//try each permutation once
//        option op = { 0,1,2,3,4,5 };
//        do
//        {
//            auto v = score(op);
//            sampler.feedback(op, v);
//            sampler.display(op);
//        } while (std::next_permutation(op.begin(), op.end()));
//    }
//
//    //stop time
//    auto atime = Clock::now();
//    auto diff = std::chrono::duration<double, std::milli >(atime - alast).count();
//    std::cout << diff << " ms";
//
//
//    //then try samples
//    for (int i = 0; i < 2000; i++)
//    {
//        auto op = sampler.get();
//        auto v = score(op);
//        sampler.feedback(op, v);
//        sampler.display(op);
//    }
//
//
//    return 0;
//
//}