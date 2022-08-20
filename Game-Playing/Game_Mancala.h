#pragma once

#include <vector>
#include <array>
#include <set>
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

	std::vector<int> GetValidActions() const
	{
		std::vector<int> validActions;
		GetValidActions(validActions);
		return validActions;
	}

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

	const std::array<int,6> AllActions = { 0,1,2,3,4,5 };

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
	

	int ActivePlayerAdvantage() const { return  ActivePlayerScore() - OpponentScore(); }

	int ActivePlayerScore() const { return BoardState[PlayerMancalaCupIx]; }
	int OpponentScore() const { return BoardState[OpponentMancalaCupIx]; }

private:
	std::array<int,14>  BoardState = { 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0 };
	std::set<int> Player1Cups= { 0,1,2,3,4,5 };
	std::set<int> Player2Cups = { 7,8,9,10,11,12 };
	int TurnNumber = 0;
	int ActivePlayer = 1;
	int PlayerMancalaCupIx = 6;  // active players mancala
	int OpponentMancalaCupIx = 13;
	std::array<int, 1000> ActionSequence = { 0 }; // assumes < 1000 actions til game ends

	

};

