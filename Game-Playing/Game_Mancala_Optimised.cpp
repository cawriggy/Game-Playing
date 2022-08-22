#include "Game_Mancala_Optimised.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <array>
#include <algorithm>

/*
    Mancala Rules
    - to play select a cup on your side
    - distribute the stones, 1 each in cups to the right
      going anti clockwise, including own mancala but not opponents mancala.
    - if end in your mancala get another turn
    - if end in empty cup on your side, capture opponents stones
      in opposite cup and add to your mancala.
*/



//
// optimisations
// deduce final score from the number in the mancala of the player who has no more beads (instead of summingthe remaining beads in a for loop)
// rearrange the boardstate so it is always from the active players perspective (avoids several branches (and makes the code tidier)
// 



void Game_Mancala_Optimised::Display() const
{
    std::cout << GetDisplayString();
}

void Game_Mancala_Optimised::DisplayActionSequence() const
{
    std::cout << GetDisplayActionSequenceString();
}

std::string Game_Mancala_Optimised::GetDisplayString() const
{
    //display the board as text with Player 2 at top, Player 1 below

    //pad numbers to width 2 (i.e. "2" becomes "2 ")
    auto stateString = [this](int i) { int n = BoardState[i]; return std::to_string(n) + ((n < 10) ? " " : ""); };

    std::string player = std::to_string(ActivePlayer);

    std::string player1_mancala = stateString(6);
    std::string player2_mancala = stateString(13);
    std::string player2_cups;
    for (int i = 12; i > 6; i--)
    {
        player2_cups += "  " + stateString(i);
    };
    std::string player1_cups;
    for (int i = 0; i < 6; i++)
    {
        player1_cups += "  " + stateString(i);
    };


    std::string s = "\n---       Player " + player + " to play      ---\n" +
        " " + player2_mancala + " |" + player2_cups + " |\n" +
        "    |" + player1_cups + " |  " + player1_mancala + "\n\n";
    return s;
}

std::string Game_Mancala_Optimised::GetDisplayActionSequenceString() const
{
    std::string result;
    auto game = Game_Mancala_Optimised();
    game.Reset();
    result += "move 0";
    result += game.GetDisplayString();
    for (int i = 0; i < TurnNumber; i++)
    {
        result += "move " + std::to_string(i + 1);
        int action = ActionSequence[i];
        game.Do(action);
        result += game.GetDisplayString();
    }
    return result;
}


void Game_Mancala_Optimised::Do(int Action)
{
    assert(IsValidAction(Action));
    assert(TurnNumber < 1000 && TurnNumber >= 0);
    
    if (!IsValidAction(Action) || (unsigned) TurnNumber >= 1000) { return; } //invalid action, do not process it


    unsigned cupIx = Action;

    //pick up beads from the chosen cup
    int held = BoardState[cupIx];
    BoardState[cupIx] = 0;


    // distribute beads
    while (held > 0)
    {
        // advance by 1 looping back round the board skipping the opponent mancala
        cupIx = (cupIx + 1) % 13;

        // drop a bead
        held -= 1;
        BoardState[cupIx] += 1;
    }
    //cup index is now the location at which the last bead was paced


    // capture if last bead dropped in an empty cup on players side
    if ((cupIx <= 5) && (BoardState[cupIx] == 1))
    {
        int opposite_cupIx = 12 - cupIx;

        BoardState[PlayerMancalaCupIx] += BoardState[opposite_cupIx];
        BoardState[opposite_cupIx] = 0;
    }

    ActionSequence[TurnNumber++] = Action;



    if (cupIx != PlayerMancalaCupIx) // end of turn (did not end move in mancala)
    {
        ActivePlayer = (1 + 2) - ActivePlayer; //switch between 1 and 2
        
        //rotate the board
        std::swap_ranges(BoardState.begin(), BoardState.begin() + 7, BoardState.begin() + 7);

    }

}

void Game_Mancala_Optimised::Undo(int Action) //TODO
{
    bool CanUndo = false;
    assert(CanUndo);
}


bool Game_Mancala_Optimised::IsValidPlayer(int p) const
{
    return p == 1 || p == 2;
}


std::string Game_Mancala_Optimised::GetName() const
{
    return "Mancala";
}

//int Game_Mancala_Optimised::GetActivePlayer() const
//{
//    assert(IsValidPlayer(ActivePlayer));
//    return ActivePlayer;
//}

int Game_Mancala_Optimised::GetTurnNumber() const
{
    assert(0 <= TurnNumber && TurnNumber <= 1000);
    return TurnNumber;
}

void Game_Mancala_Optimised::GetValidActions(std::vector<int>& OutActions) const
{
    OutActions.reserve(6);
    for (int Action : AllActions)
    {
        if (IsValidAction(Action))
        {
            OutActions.push_back(Action);
        }
    }
}

bool Game_Mancala_Optimised::IsValidAction(int Action) const
{
    return (0 <= Action && Action <= 5) && (BoardState[Action] != 0);
}

Game::PlayState Game_Mancala_Optimised::GetPlayState() const
{
    //an available action means the game is unfinished
    for (auto action : AllActions) 
    {
        if (BoardState[action] != 0)
        {
            return Unfinished;
        }
    }

    // no actions available for the active player
    // opponents gets all remaining points
    int active_player_score = BoardState[6];
    int opponent_score = 48 - active_player_score;


    if (active_player_score != opponent_score) // not a tie
    {
        if ((active_player_score > opponent_score) != (ActivePlayer == 1))
        {
            return Player2Wins;
        }
        else
        {
            return Player1Wins;
        }
    }
    return Tie;

}

void Game_Mancala_Optimised::Reset()
{
    TurnNumber = 0;
    ActivePlayer = 1;

    // reset the board (4 in each cup, 0 in both mancalas)
    std::fill(std::begin(BoardState), std::end(BoardState), 4);
    BoardState[6] = 0;
    BoardState[13] = 0;

    ActionSequence = { 0 };
}


std::vector<int> Game_Mancala_Optimised::GetStateVector() const
{
    std::vector<int> stateVec(BoardState.begin(), BoardState.end());
    stateVec.push_back(ActivePlayer);
    return stateVec;
}


