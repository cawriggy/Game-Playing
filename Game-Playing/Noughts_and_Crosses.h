#pragma once

#include <vector>
#include "Game.h"
#include <string>


class Noughts_and_Crosses:
	public Game
{
public:
	//game interface
	std::string GetName() const;
	int GetActivePlayer() const;
	int GetTurnNumber() const;
	void GetValidActions(std::vector<int>& OutValidActions) const;
	void Act(int Act) override;
	PlayState GetPlayState() const;
	void Reset();
	std::unique_ptr<Game> Clone() const { return std::make_unique<Noughts_and_Crosses>(*this); }
	std::vector<int> GetStateVector() const;

	// visualisation
	void Display() const;
	void DisplayActionSequence() const;
	std::string GetDisplayString() const;
	std::string GetDisplayActionSequenceString() const;

	bool IsWinningState() const;
	const int AllActions[9] = { 0,1,2,3,4,5,6,7,8 };
	
	// validation
	bool IsValidAction(int Action) const;
	bool IsValidCell(int i) const;
	bool IsValidPlayer(int p) const;

	// BoardState layout
	//   0 1 2
	//   3 4 5
	//   6 7 8
	enum cell
	{
		topLeft,
		topMid,
		topRight,
		midLeft,
		midMid,
		midRight,
		botLeft,
		botMid,
		botRight
	};

private:

	int BoardState[9] = { 0,0,0,0,0,0,0,0,0 };
	int TurnNumber = 0;
	int ActivePlayer = 0;

	const int WinGroups[8][3] =
	{ {0,1,2}, {3,4,5}, {6,7,8}, //horizontal
	  {0,3,6}, {1,4,7}, {2,5,8}, //vertical
	  {0,4,8}, {2,4,6} }; //diagonal

	int ActionSequence[9] = { 0,0,0,0,0,0,0,0,0 };

};

