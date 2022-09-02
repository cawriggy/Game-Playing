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
    std::string s;

    //calculate a msk for winning any pieces so they can be capitalised
    auto winMask = WinningPieceMask();

    //for every rows (top to bottom)
    for (int row = 5; row >= 0; row--)
    {
        s += '\n';//new line for the row

        //for every col (left to right)
        for (int col = 0; col < 7; col++)
        {
            //determine the bit corresponding to row,col
            int i = row * 7 + col;
            auto bit = std::uint_fast64_t(1) << i;
            assert(IsValidCell(bit));

            //determine which player (if any) owns the cell
            int t = 0;
            if (ActivePlayerOwned & bit) { t = GetActivePlayer(); }
            if (InactivePlayerOwned & bit) { t = 3 - GetActivePlayer(); }
            assert(t == 0 || t == 1 || t == 2);

            //capitalise winning pieces
            bool bInWinningLine = winMask & bit;
            char c = bInWinningLine ? "_AB"[t] : "_ab"[t];

            s += c;
            s += ' ';
        }
    }
    s += '\n';

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

    bool validColumn = (0 <= i && i <= 6);
    if (!validColumn) { return false; }

    bool columHasSpace = (0 < NextSpace[i] && NextSpace[i] < std::uint_fast64_t(1) << 42);
    return columHasSpace;
}

void Game_Connect4::Do(int Action)
{
    assert(IsValidAction(Action));
    if (!(GetPlayState() == Game::Unfinished))
    {
        throw("can't act as game already finished");
    }
    assert(TurnNumber < 42 && TurnNumber >= 0);

    auto nextOwned = ActivePlayerOwned | NextSpace[Action];
    ActivePlayerOwned = InactivePlayerOwned;
    InactivePlayerOwned = nextOwned;

    NextSpace[Action] <<= 7; //next time use the bit 7 spaces further on

    ActionSequence[TurnNumber++] = Action;
    ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
}

void Game_Connect4::Undo(int Action)
{
    TurnNumber--;

    assert(TurnNumber < 42 && TurnNumber >= 0);
    assert(ActionSequence[TurnNumber] == Action);

    NextSpace[Action] >>= 7;

    //undo the last action
    auto tempOwned = InactivePlayerOwned ^ NextSpace[Action];

    //swap the active and inactive
    InactivePlayerOwned = ActivePlayerOwned;
    ActivePlayerOwned = tempOwned;

    ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
}



bool Game_Connect4::IsValidCell(std::uint_fast64_t i) const
{
    bool usesOnlyValidBits = i < (std::uint_fast64_t(1) << 42);
    bool usesOnly1Bit = (i & (i - 1)) == std::uint_fast64_t(0);
    return (usesOnlyValidBits && usesOnly1Bit);
}

bool Game_Connect4::IsValidPlayer(int p) const
{
    return p == 1 || p == 2;
}

std::uint_fast64_t Game_Connect4::WinningPieceMask() const
{
    //calculate a msk for winning any pieces so they can be capitalised
    auto winMask = std::uint_fast64_t(0);
    auto owned = InactivePlayerOwned;
    for (auto& [s, m] : win_directionMaskTuple)
    {
        auto pairs = owned & (owned >> s);
        auto pairs_of_pairs = pairs & (pairs >> (s + s));
        pairs_of_pairs &= m;
        auto line_winning = pairs_of_pairs | (pairs_of_pairs << s);
        line_winning |= line_winning << (s + s);
        winMask |= line_winning;
    }
    owned = ActivePlayerOwned;
    for (auto& [s, m] : win_directionMaskTuple)
    {
        auto pairs = owned & (owned >> s);
        auto pairs_of_pairs = pairs & (pairs >> (s + s));
        pairs_of_pairs &= m;
        auto line_winning = pairs_of_pairs | (pairs_of_pairs << s);
        line_winning |= line_winning << (s + s);
        winMask |= line_winning;
    }

    return winMask;
}


Game_Connect4::Game_Connect4()
{
    // BoardState layout  7 x 6
    //	35 36 37 38 39 40 41
    //	28 29 30 31 32 33 34
    //  21 22 23 24 25 26 27
    //  14 15 16 17 18 19 20
    //	7  8  9  10 11 12 13
    //  0  1  2  3  4  5  6

    // calculate parameters for win checks
    // for each direction
    //    a shift amount (the next bit in any direction is at a constant offset e.g +7 for up)
    //    a mask to avoid wrap-around (i.e. 19,20,21,22 is not a way to win)
    //    mask indicates cells which have enough space in the direction

    //directions are: up, right, diagonally up-right, diagonally up-left
    const int u = 7;
    const int r = 1;
    const int ur = u + r;
    const int ul = u - r;

    std::uint_fast64_t mu = 0;
    std::uint_fast64_t mr = 0;
    std::uint_fast64_t mur = 0;
    std::uint_fast64_t mul = 0;

    //generate mask indicating which cells have space for a line of 4 in the specified direction
    for (int i = 0; i < 42; i++)
    {
        auto bit = std::uint_fast64_t(1) << i;
        if (i < 21) { mu |= bit; }
        if (i % 7 < 4) { mr |= bit; }
        if ((i < 21) && (i % 7 < 4)) { mur |= bit; }
        if ((i < 21) && (i % 7 > 2)) { mul |= bit; }
    }

    win_directionMaskTuple[0] = { u, mu };
    win_directionMaskTuple[1] = { r, mr };
    win_directionMaskTuple[2] = { ur, mur };
    win_directionMaskTuple[3] = { ul, mul };
}

bool Game_Connect4::IsWinningState() const
{
    //only need to check most recent move i.e. inactive player
    auto owned = InactivePlayerOwned;

    //use masks and shifts to check for 4 in a line
    // shift indicates direction of the line of 4
    // mask indicates which cells have space (to prevent wrapping round to the next row from causing win)
    for (auto& [s, m] : win_directionMaskTuple)
    {
        auto pairs = owned & (owned >> s);
        auto pairs_of_pairs = pairs & (pairs >> (s + s));
        if (pairs_of_pairs & m) { return true; }
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
    OutActions.reserve(7);
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
        int lastToAct = GetActivePlayer();
        return lastToAct == 1 ? Player2Wins : Player1Wins;
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
    ActivePlayerOwned = 0;
    InactivePlayerOwned = 0;
    for (int i = 0; i < 7; i++)
    {
        NextSpace[i] = { std::uint_fast64_t(1) << i };
    }

}


std::vector<int> Game_Connect4::GetStateVector() const
{
    //knowing the locations owned by each players uniquely defines the state
    //make an int vector from this data

    std::vector<int> arr;
    arr.reserve(4);

    //split big ints accross multiple ints
    for (int i = 0; i < sizeof(std::uint_fast64_t); i += sizeof(int))
    {
        arr.push_back(int(ActivePlayerOwned >> 8 * sizeof(int) * i));
    }
    for (int i = 0; i < sizeof(std::uint_fast64_t); i += sizeof(int))
    {
        arr.push_back(int(InactivePlayerOwned >> 8 * sizeof(int) * i));
    }

    return arr;
}

