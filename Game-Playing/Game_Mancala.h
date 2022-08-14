#pragma once

#include <vector>
#include "Game.h"
#include <string>


class Game_Mancala :
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
	std::unique_ptr<Game> Clone() const { return std::make_unique<Game_Mancala>(*this); }
	std::vector<int> GetStateVector() const;

	// visualisation
	void Display() const;
	void DisplayActionSequence() const;
	std::string GetDisplayString() const;
	std::string GetDisplayActionSequenceString() const;

	const int AllActions[6] = { 0,1,2,3,4,5 }; 

	// validation
	bool IsValidAction(int Action) const;
	bool IsValidPlayer(int p) const;

	// BoardState layout 
	//  
	// Mancala board has two rows of 6 cups initially containing 
	// 4 stones each and a Mancala either end eg
	//	0    4 4 4 4 4 4     
	//	     4 4 4 4 4 4    0 
	// 
	// 13  12 11 10 9  8  7         - opponent scores in cup 13
	//     0  1  2  3  4  5     6   - active player scores in 6 and chooses from cups 0 - 5
	

private:
	int BoardState[14] = { 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0 }; 

	//void FillCup(int cup, int p) { BoardState[cup] += p; }
//	void EmptyCup(int cup) { BoardState[cup] = 0; }
//	int GetCup(int cup) { return BoardState[cup]; }
	int PickUpBeads(int cup) { int beads = BoardState[cup]; BoardState[cup] = 0; return beads; }
	void SwitchPlayer() {
		ActivePlayer = 3 - ActivePlayer; //switch between 1 and 2
		// switch board round
		std::vector<int> BoardStateCopy = GetStateVector();
		for (int i = 0; i < 14; i++)
		{
			BoardState[i] = BoardStateCopy[(i + 7) % 14];
		}
	}
	
	int TurnNumber = 0;
	int ActivePlayer = 1;
	int ActionSequence[1000] = { 0 }; // assumes < 1000 actions til game ends

};

