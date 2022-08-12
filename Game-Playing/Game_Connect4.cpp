#include "Game_Connect4.h"
#include <iostream>
#include <assert.h>
#include <vector>

void Game_Connect4::Display() const
{
    std::cout << GetDisplayString();
}

void Game_Connect4::DisplayActionSequence() const
{
    std::cout << GetDisplayActionSequenceString();
}

std::string Game_Connect4::GetDisplayString() const
{
    //display the board as text
    int i = 0;
    char s[] = "\n_ _ _ _ _ _ _\n_ _ _ _ _ _ _\n_ _ _ _ _ _ _\n_ _ _ _ _ _ _\n_ _ _ _ _ _ _\n_ _ _ _ _ _ _\n\n";
    for (char& c : s)
    {
        if (c == '_')
        {
            assert(IsValidCell(i));
            int t = BoardState[i];
            assert(t == 0 || t == 1 || t == 2);
            c = "_12"[t];
            i++;
        }
    }
    return s;
}

std::string Game_Connect4::GetDisplayActionSequenceString() const 
{
    std::string result;
    auto game = Game_Connect4();
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

bool Game_Connect4::IsValidAction(int Action) const 
{
    const int i = Action;
    if (i > 6 || 0 > i) return false;
    return BoardState[i] == 0;
}

void Game_Connect4::Do(int Action) 
{
    assert(IsValidAction(Action));
    if (!(GetPlayState() == Game::Unfinished)) 
    { 
        throw("can't act as game already finished"); 
    }
    const int p = GetActivePlayer();
    assert(p == 1 || p == 2);// "invalid player number"
    assert(TurnNumber < 42 && TurnNumber >= 0);
    int row = 5;
    while (BoardState[row * 7 + Action])
        row--;
    BoardState[row * 7 + Action] = p; // fill from bottom
    ActionSequence[TurnNumber++] = Action;
    ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
}

void Game_Connect4::Undo(int Action)
{
    TurnNumber--;
    assert(TurnNumber < 42 && TurnNumber >= 0);
    assert(ActionSequence[TurnNumber] == Action);
    const int p = GetActivePlayer();
    assert(p == 1 || p == 2);// "invalid player number"

    int row = 5;
    while (BoardState[row * 7 + Action])
    {
        row--;
    }
    row++; //go back one space

    BoardState[row * 7 + Action] = 0; // clear from to top
    ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
}


bool Game_Connect4::IsValidCell(int i) const 
{
    return i < 42 && i >= 0;
}

bool Game_Connect4::IsValidPlayer(int p) const 
{
    return p == 1 || p == 2;
}


bool Game_Connect4::IsWinningState() const 
{
    //for each line of 4
    for (const int* g : WinGroups)  //
    {
        assert(IsValidCell(g[0]));
        assert(IsValidCell(g[1]));
        assert(IsValidCell(g[2]));
        assert(IsValidCell(g[3]));

        int a = BoardState[g[0]];
        int b = BoardState[g[1]];
        int c = BoardState[g[2]];
        int d = BoardState[g[3]];

        //winning state if all 4 are owned by same player
        if ((a == b) && (b == c) && (c == d) && (d != 0))
        {
            return true;
        }
    }
    return false;
}

std::string Game_Connect4::GetName() const
{
    return "Connect4";
}

int Game_Connect4::GetActivePlayer() const
{
    assert(IsValidPlayer(ActivePlayer));
    return ActivePlayer;
}

int Game_Connect4::GetTurnNumber() const 
{
    assert(TurnNumber >= 0 && TurnNumber <= 42);
    return TurnNumber;
}

void Game_Connect4::GetValidActions(std::vector<int>& OutActions) const 
{
    OutActions.reserve(42);
    for (int Action : AllActions)
    {
        if (IsValidAction(Action))
        {
            OutActions.push_back(Action);
        }
    }
}

Game::PlayState Game_Connect4::GetPlayState() const 
{
    if (IsWinningState())
    {
        int NextToAct = GetActivePlayer();
        return NextToAct == 1 ? Player2Wins : Player1Wins;
    }
    if (TurnNumber >= 42) 
    {
        return Tie;
    }
    return Unfinished;
}

void Game_Connect4::Reset()
{
    TurnNumber = 0;
    ActivePlayer = 1;
    std::fill(std::begin(BoardState), std::end(BoardState), 0);
}


std::vector<int> Game_Connect4::GetStateVector() const
{
    return std::vector<int>(std::begin(BoardState), std::end(BoardState));
}

