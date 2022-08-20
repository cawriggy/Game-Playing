#pragma once

#include <vector>
#include <string>
#include <memory>
#include <iostream>

class Game
{
public:
	enum PlayState
	{
		Tie = 0,
		Player1Wins = 1,
		Player2Wins = 2,
		Unfinished = -1,
	};

	virtual std::string GetName() const = 0;
	virtual int GetActivePlayer() const = 0;
	virtual int GetTurnNumber() const = 0;
	virtual PlayState GetPlayState() const = 0;

	virtual void GetValidActions(std::vector<int>& OutValidActions) const = 0;
	std::vector<int> GetValidActions() const 
	{
		std::vector<int> validActions;
		GetValidActions(validActions);
		return validActions;
	}

	
	virtual void Do(int Act) = 0;
	virtual void Undo(int Action) = 0;
	virtual void Reset() = 0;
	virtual ~Game() = default;
	virtual std::unique_ptr<Game> Clone() const = 0;
	virtual std::vector<int> GetStateVector() const = 0;

	virtual std::string GetDisplayString() const { return "get display string not implemented"; }
	void Display() const
	{
		std::cout << GetDisplayString();
	}
};
