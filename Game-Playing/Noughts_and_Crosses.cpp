#include "Noughts_and_Crosses.h"
#include <iostream>
#include <assert.h>
#include <vector>
#include <bit>


void Noughts_and_Crosses::Display() const
{
    std::cout << GetDisplayString();
}

void Noughts_and_Crosses::DisplayActionSequence() const
{
    std::cout << GetDisplayActionSequenceString();
}

std::string Noughts_and_Crosses::GetDisplayString() const
{
    //display the board as text
    int i = 0;
    char s[] = "\n_ _ _\n_ _ _\n_ _ _\n\n";
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

std::string Noughts_and_Crosses::GetDisplayActionSequenceString() const
{
    std::string result;
    auto game = Noughts_and_Crosses();
    game.Reset();
    result += "move 0";
    result += game.GetDisplayString();
    for (int i = 0; i < TurnNumber; i++)
    {
        result += "move " + std::to_string(i + 1);
        int action = ActionSequence[i];
        game.Act(action);
        result += game.GetDisplayString();
    }
    return result;
}

bool Noughts_and_Crosses::IsValidAction(int Action) const
{
    const int i = Action;
    if (!IsValidCell(i)) return false;
    return BoardState[i] == 0;
}


void Noughts_and_Crosses::Act(int Action)
{
    assert(IsValidAction(Action));
    const int p = GetActivePlayer();
    assert(p == 1 || p == 2);// "invalid player number"
    assert(GetPlayState() == Game::Unfinished);
    assert(TurnNumber < 9 && TurnNumber >= 0);
    BoardState[Action] = p;
    ActionSequence[TurnNumber++] = Action;
    ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
}


bool Noughts_and_Crosses::IsValidCell(int i) const
{
    return i < 9 && i >= 0;
}

bool Noughts_and_Crosses::IsValidPlayer(int p) const
{
    return p==1 || p==2;
}


bool Noughts_and_Crosses::IsWinningState() const
{
    //for each line of 3
    for (const int* g : WinGroups)
    {
        assert(IsValidCell(g[0]));
        assert(IsValidCell(g[1]));
        assert(IsValidCell(g[2]));

        int a = BoardState[g[0]];
        int b = BoardState[g[1]];
        int c = BoardState[g[2]];

        //winning state if all 3 are owned by same player
        if ((a == b) && (b == c) && (c != 0))
        {
            return true;
        }
    }
    return false;
}

std::string Noughts_and_Crosses::GetName() const
{
    return "Noughts-and-Crosses";
}

int Noughts_and_Crosses::GetActivePlayer() const
{
    assert(IsValidPlayer(ActivePlayer));
    return ActivePlayer;
}

int Noughts_and_Crosses::GetTurnNumber() const
{
    assert(TurnNumber >= 0 && TurnNumber <= 9);
    return TurnNumber;
}

void Noughts_and_Crosses::GetValidActions(std::vector<int>& OutActions) const
{
    //its faster to reserve what you need upfront, you can shrink to fit later if you need
    OutActions.reserve(9);
    for (int Action: AllActions)
    {
        if (IsValidAction(Action)) 
        {
            OutActions.push_back(Action);
        }
    }
    //OutActions.shrink_to_fit();
}

Game::PlayState Noughts_and_Crosses::GetPlayState() const
{
    if (IsWinningState())
    {
        int NextToAct = GetActivePlayer();
        return NextToAct == 1 ? Player2Wins : Player1Wins;
    }
    if (TurnNumber >= 9) 
    {
        return Tie; 
    }
    return Unfinished;
}

void Noughts_and_Crosses::Reset()
{
    TurnNumber = 0;
    ActivePlayer = 1;
    std::fill(std::begin(BoardState), std::end(BoardState), 0);
}

std::vector<int> Noughts_and_Crosses::GetStateVector() const
{
    return std::vector<int>(std::begin(BoardState), std::end(BoardState));
}


