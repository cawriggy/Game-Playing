#pragma once

#include <vector>
#include "Game.h"
#include <string>


class Game_Connect4 :
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
	std::unique_ptr<Game> Clone() const { return std::make_unique<Game_Connect4>(*this); }
	std::vector<int> GetStateVector() const;

	// visualisation
	void Display() const;
	void DisplayActionSequence() const;
	std::string GetDisplayString() const;
	std::string GetDisplayActionSequenceString() const;

	bool IsWinningState() const;
	const int AllActions[7] = { 0,1,2,3,4,5,6 };

	// validation
	bool IsValidAction(int Action) const;
	bool IsValidCell(int i) const;
	bool IsValidPlayer(int p) const;

	// BoardState layout  7 x 6
	//  0  1  2  3  4  5  6
	//	7  8  9  10 11 12 13
	//  14 15 16 17 18 19 20
	//  21 22 23 24 25 26 27
	//	28 29 30 31 32 33 34
	//	35 36 37 38 39 40 41


private:

	int BoardState[42] = { 0 };
	int TurnNumber = 0;
	int ActivePlayer = 1;
	int ActionSequence[42] = { 0 };

	const int WinGroups[69][4] = {
	 {0, 1, 2, 3},
	 {7, 8, 9, 10},
	 {14, 15, 16, 17},
	 {21, 22, 23, 24},
	 {28, 29, 30, 31},
	 {35, 36, 37, 38},
	 {1, 2, 3, 4},
	 {8, 9, 10, 11},
	 {15, 16, 17, 18},
	 {22, 23, 24, 25},
	 {29, 30, 31, 32},
	 {36, 37, 38, 39},
	 {2, 3, 4, 5},
	 {9, 10, 11, 12},
	 {16, 17, 18, 19},
	 {23, 24, 25, 26},
	 {30, 31, 32, 33},
	 {37, 38, 39, 40},
	 {3, 4, 5, 6},
	 {10, 11, 12, 13},
	 {17, 18, 19, 20},
	 {24, 25, 26, 27},
	 {31, 32, 33, 34},
	 {38, 39, 40, 41},
	 {0, 8, 16, 24},
	 {1, 9, 17, 25},
	 {2, 10, 18, 26},
	 {3, 11, 19, 27},
	 {7, 15, 23, 31},
	 {8, 16, 24, 32},
	 {9, 17, 25, 33},
	 {10, 18, 26, 34},
	 {14, 22, 30, 38},
	 {15, 23, 31, 39},
	 {16, 24, 32, 40},
	 {17, 25, 33, 41},
	 {3, 9, 15, 21},
	 {4, 10, 16, 22},
	 {5, 11, 17, 23},
	 {6, 12, 18, 24},
	 {10, 16, 22, 28},
	 {11, 17, 23, 29},
	 {12, 18, 24, 30},
	 {13, 19, 25, 31},
	 {17, 23, 29, 35},
	 {18, 24, 30, 36},
	 {19, 25, 31, 37},
	 {20, 26, 32, 38},
	 {0, 7, 14, 21},
	 {1, 8, 15, 22},
	 {2, 9, 16, 23},
	 {3, 10, 17, 24},
	 {4, 11, 18, 25},
	 {5, 12, 19, 26},
	 {6, 13, 20, 27},
	 {7, 14, 21, 28},
	 {8, 15, 22, 29},
	 {9, 16, 23, 30},
	 {10, 17, 24, 31},
	 {11, 18, 25, 32},
	 {12, 19, 26, 33},
	 {13, 20, 27, 34},
	 {14, 21, 28, 35},
	 {15, 22, 29, 36},
	 {16, 23, 30, 37},
	 {17, 24, 31, 38},
	 {18, 25, 32, 39},
	 {19, 26, 33, 40},
	 {20, 27, 34, 41}
		};

};


