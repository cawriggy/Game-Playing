#include "Game_Mancala.h"
#include <iostream>
#include <assert.h>
#include <vector>

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
    //display the board as text
    std::vector<int> BoardStateCopy = GetStateVector();
    if (GetActivePlayer() == 2)
    {
        for (int i = 0; i < 14; i++)
        {
            BoardStateCopy[i] = BoardState[(i + 7) % 14];
        }
    }
    
    auto cupString = [BoardStateCopy](int i) {int n = BoardStateCopy[i]; return std::to_string(n) + ((n < 10) ? " " : ""); };

    std::string player = std::to_string(GetActivePlayer());
    
    std::string opponent_mancala = cupString(13);
    std::string opponent_cups;
    for (int i = 12; i > 6; i--)
    {
        opponent_cups += "  " + cupString(i);
    };
    std::string player_cups;
    for (int i = 0; i < 6; i++)
    {
        player_cups += "  " + cupString(i);
    };
    std::string player_mancala = " " + cupString(6);

    std::string s = "\n---       Player " + player + " to play      ---\n" +
        " " + opponent_mancala + " |" + opponent_cups + " |\n" +
        "    |" + player_cups   + " | " + player_mancala + "\n\n";
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
    if (i > 5 || 0 > i) return false;
    return BoardState[i] != 0;
}

void Game_Mancala::Do(int Action) 
{
    int cup = Action;

    assert(IsValidAction(Action));
    assert(TurnNumber < 1000 && TurnNumber >= 0);

    //pick up beads from the chosen cup
    int held = PickUpBeads(cup);
    // distribute beads
    while (held > 0)
    {
        // advance by 1 but loop back at opponents mancala
        cup = (cup + 1) % 13;
        // drop a bead
        held -= 1;
        BoardState[cup] += 1;
    }
    // capture if last bead dropped in an empty cup on players side
    if (BoardState[cup] == 1 && cup < 6)
    {
        int opponent_cup = 12 - cup;
        BoardState[6] += BoardState[opponent_cup];
        BoardState[opponent_cup] = 0;
    }

    ActionSequence[TurnNumber++] = Action;

    if (cup != 6) // did not finish in players mancala
    { 
        SwitchBoard();
        ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
    }
    
}

void Game_Mancala::Undo(int Action) //TODO
{
    TurnNumber--;
    const int p = GetActivePlayer();
    int cup = Action;

    assert(TurnNumber >= 0);
    assert(ActionSequence[TurnNumber] == Action);
    assert(p == 1 || p == 2);// "invalid player number"

    //TODO

    //ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
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
        int active_player_mancala = BoardState[6];
        // sweep remaining into opponents mancala
        int opponent_mancala = BoardState[13];
        for (int i = 7; i < 13; i++)
        {
            opponent_mancala += BoardState[i];
        }
        assert(opponent_mancala + active_player_mancala == 48);
        if (active_player_mancala != opponent_mancala) // not a tie
        {
            if (active_player_mancala > opponent_mancala)
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
    std::fill(std::begin(BoardState), std::end(BoardState), 4);
    // empty the mancala cups
    BoardState[6] = 0; 
    BoardState[13] = 0;
}


std::vector<int> Game_Mancala::GetStateVector() const 
{
    return std::vector<int>(std::begin(BoardState), std::end(BoardState));
}


