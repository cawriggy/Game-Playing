#pragma once

#include <vector>
#include "Game.h"
#include <string>
#include <cinttypes>



/// <summary>
/// A fast connect 4 implementation using a compact board representation
/// </summary>
class Game_Connect4_Bitboards :
	public Game
{
public:
	//game interface
	std::string GetName() const;
	int GetActivePlayer() const;
	int GetTurnNumber() const;
	void GetValidActions(std::vector<int>& OutValidActions) const;
	void Do(int action) override;
	void Undo(int action) override;
	PlayState GetPlayState() const;
	void Reset();
	std::unique_ptr<Game> Clone() const { return std::make_unique<Game_Connect4_Bitboards>(*this); }
	std::vector<int> GetStateVector() const;

	// visualisation
	void Display() const;
	void DisplayActionSequence() const;
	std::string GetDisplayString() const;
	std::string GetDisplayActionSequenceString() const;

	Game_Connect4_Bitboards();
	bool IsWinningState() const;
	const int AllActions[7] = { 0,1,2,3,4,5,6 };

	// validation
	bool IsValidAction(int Action) const;
	bool IsValidCell(std::uint_fast64_t i) const;
	bool IsValidPlayer(int p) const;

	// BoardState layout  7 x 6
	//	35 36 37 38 39 40 41
	//	28 29 30 31 32 33 34
	//  21 22 23 24 25 26 27
	//  14 15 16 17 18 19 20
	//	7  8  9  10 11 12 13
	//  0  1  2  3  4  5  6

	// each cell corresponds to 1 bit in a 64bit int
	// use 2  64 bit ints (1 for each player)
	// 1s represent owned cells


private:
	std::uint_fast64_t ActivePlayerOwned = std::uint_fast64_t(0);
	std::uint_fast64_t InactivePlayerOwned = std::uint_fast64_t(0);
	std::uint_fast64_t NextSpace[7] = { 1<<0, 1<<1, 1<<2, 1<<3, 1<<4, 1<<5, 1<<6 };
	std::tuple<int, std::uint_fast64_t> win_directionMaskTuple[4];

	int TurnNumber = 0;
	int ActivePlayer = 1;
	int ActionSequence[42] = { 0 };

	std::uint_fast64_t WinningPieceMask() const;
};



