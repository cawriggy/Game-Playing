#pragma once

#include "Game.h"
#include <vector>
#include <array>
#include <string>
#include <assert.h>


class Game_Mancala :
	public Game
{
public:
	//game interface
	std::string GetName() const;

	int GetActivePlayer() const
	{
		assert(IsValidPlayer(ActivePlayer));
		return ActivePlayer;
	}

	int GetTurnNumber() const;
	void GetValidActions(std::vector<int>& OutValidActions) const;

	std::vector<int> GetValidActions() const
	{
		std::vector<int> validActions;
		GetValidActions(validActions);
		return validActions;
	}

	void Do(int action) override;

	[[deprecated("make copies instead")]]
	void Undo(int action) override;

	PlayState GetPlayState() const;
	void Reset();
	std::unique_ptr<Game> Clone() const { return std::make_unique<Game_Mancala>(*this); }
	std::vector<int> GetStateVector() const;

	const std::array<int, 6> AllActions = { 5,4,3,2,1,0 };

	// visualisation
	void SetPerspective(int player) { persepective = player; }
	void Display() const;
	void DisplayActionSequence() const;
	std::string GetDisplayString() const;
	std::string GetDisplayActionSequenceString() const;

	// validation
	bool IsValidAction(int Action) const;
	bool IsValidPlayer(int p) const;

	//scoring
	int ActivePlayerAdvantage() const { return  ActivePlayerScore() - OpponentScore(); }
	int ActivePlayerScore() const { return BoardState[PlayerMancalaCupIx]; }
	int OpponentScore() const { return BoardState[OpponentMancalaCupIx]; }


	// BoardState layout 
	//  
	// Mancala board has two rows of 6 cups initially containing 
	// 4 stones each and a Mancala either end eg
	//	0    4 4 4 4 4 4     
	//	     4 4 4 4 4 4    0 
	// 
	// 13  12 11 10 9  8  7         - opponent scores in cup 13
	//     0  1  2  3  4  5     6   - active player scores in 6 and chooses from cups 0 - 5

	// the board is stored from the perspective of the active player (6 active player's cups, 1 active player's mancala, 6 opponent's cups, 1 opponent's mancala)


private:
	std::array<int, 14>  BoardState = { 4, 4, 4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 0 };
	int TurnNumber = 0;
	int ActivePlayer = 1;
	const int PlayerMancalaCupIx = 6;  // active players mancala
	const int OpponentMancalaCupIx = 13;
	int persepective = 1;

	std::array<int, 200> ActionSequence = { 0 };
	// note: in 1 million random games at most 107 actions were used



};



