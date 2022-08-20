#include "Game_Mancala.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <array>
#include <set>

/*
    Mancala Rules
    - to play select a cup on your side
    - distribute the stones, 1 each in cups to the right
      going anti clockwise, including own mancala but not opponents mancala.
    - if end in your mancala get another turn
    - if end in empty cup on your side, capture opponents stones 
      in opposite cup and add to your mancala.
*/

void Game_Mancala::Display() const
{
    std::cout << GetDisplayString();
}

void Game_Mancala::DisplayActionSequence() const
{
    std::cout << GetDisplayActionSequenceString();
}

std::string Game_Mancala::GetDisplayString() const
{
    //display the board as text with Player 2 at top, Player 1 below
    
    auto stateString = [this](int i) { int n = BoardState[i]; return std::to_string(n) + ((n < 10) ? " " : ""); };

    std::string player = std::to_string(GetActivePlayer());

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
        "    |" + player1_cups   + " |  " + player1_mancala + "\n\n";
    return s;
}

std::string Game_Mancala::GetDisplayActionSequenceString() const
{
    std::string result; 
    auto game = Game_Mancala();
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

bool Game_Mancala::IsValidAction(int Action) const
{
    const int i = Action;
    if (0 > i || i > 5) // actions selected 0 - 5
    {
        return false;
    }
    int cupIx = (GetActivePlayer() == 1) ? i : i + 7;
    return (BoardState[cupIx] != 0);
}

void Game_Mancala::Do(int Action) 
{
    int action = Action;

    assert(IsValidAction(Action));
    assert(TurnNumber < 1000 && TurnNumber >= 0);

    int cupIx = (GetActivePlayer() == 1) ? action : action + 7;

    //pick up beads from the chosen cup
    int held = BoardState[cupIx]; 
    BoardState[cupIx] = 0;
    // distribute beads
    while (held > 0)
    {
        // advance by 1 looping round the board
        cupIx = (cupIx + 1) % 14;
        // drop a bead
        if (cupIx != OpponentMancalaCupIx)
        {
            held -= 1;
            BoardState[cupIx] += 1;
        }
    }
    // capture if last bead dropped in an empty cup on players side
    assert(0 <= cupIx && cupIx <= (14 - 1));
    if (BoardState[cupIx] == 1)
    {
        if ( (GetActivePlayer() == 1 && Player1Cups.contains(cupIx)) || (GetActivePlayer() == 2 && Player2Cups.contains(cupIx)) )
        {
            int opposite_cupIx = 12 - cupIx;
            BoardState[PlayerMancalaCupIx] += BoardState[opposite_cupIx];
            BoardState[opposite_cupIx] = 0;
        }
    }
    
    ActionSequence[TurnNumber++] = Action;

    if (cupIx != PlayerMancalaCupIx) // end of turn
    { 
        ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
        PlayerMancalaCupIx = 6 + 13 - PlayerMancalaCupIx;
        OpponentMancalaCupIx = 6 + 13 - OpponentMancalaCupIx;
    }

}

void Game_Mancala::Undo(int Action) //TODO
{
    bool CanUndo = false;
    assert(CanUndo);
}


bool Game_Mancala::IsValidPlayer(int p) const 
{
    return p == 1 || p == 2;
}


std::string Game_Mancala::GetName() const 
{
    return "Mancala";
}

int Game_Mancala::GetActivePlayer() const 
{
    assert(IsValidPlayer(ActivePlayer));
    return ActivePlayer;
}

int Game_Mancala::GetTurnNumber() const 
{
    assert(TurnNumber >= 0 && TurnNumber <= 1000);
    return TurnNumber;
}

void Game_Mancala::GetValidActions(std::vector<int>&OutActions) const 
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


Game::PlayState Game_Mancala::GetPlayState() const
{
    std::vector<int> validActions;
    GetValidActions(validActions);
    if (validActions.empty()) // Game over 
    { 
        assert(validActions.size() == 0);
        int active_player_score = ActivePlayerScore();
        int opponent_score = OpponentScore();
        // sweep remaining into opponents mancala
        if (GetActivePlayer() == 1)
        {
            for (int i = 7; i < 13; i++)
            {
                opponent_score += BoardState[i];
            }
        }
        else
        {
            for (int i = 0; i < 6; i++)
            {
                opponent_score += BoardState[i];
            }
        }
        
        assert(opponent_score + active_player_score == 48);
        if (active_player_score != opponent_score) // not a tie
        {
            if (active_player_score > opponent_score)
            {
                return GetActivePlayer() == 1 ? Player1Wins : Player2Wins;
            }
            else
            {
                return GetActivePlayer() == 1 ? Player2Wins : Player1Wins;
            }
        }
        return Tie;
        
    }
    return Unfinished;
}

void Game_Mancala::Reset() 
{
    TurnNumber = 0;
    ActivePlayer = 1;
    PlayerMancalaCupIx = 6;  // active players mancala
    OpponentMancalaCupIx = 13;
    ActionSequence = { 0 };
    std::fill(std::begin(BoardState), std::end(BoardState), 4);
    // empty the mancala cups
    BoardState[6] = 0; 
    BoardState[13] = 0;
}


std::vector<int> Game_Mancala::GetStateVector() const
{
    std::vector<int> stateVec(BoardState.begin(), BoardState.end() );
    stateVec.push_back(GetActivePlayer());
    return stateVec;
}


