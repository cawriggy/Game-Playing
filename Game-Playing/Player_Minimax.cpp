#include "Player_Minimax.h"
#include <assert.h>
#include <cstdlib> //for rand


//
// optimisation notes
// retuning a constant 0 instead of a random playout is faster and as good (at least for noughts and crosses
// act and undo act can be faster than cloning the game state
//


int Player_Minimax::ChooseAction(const Game& game)
{
	//choose an action to maximise minimum score
	int bestScore = static_cast<int>(- 1e9);
	int bestAction = -1;
	auto locGame = game.Clone();

	std::vector<int> validActions;
	game.GetValidActions(validActions);
	for (int action : validActions)
	{
		locGame->Do(action);
		int score = MinmaxState(*locGame, GetDepthLimit());
		locGame->Undo(action);
		
		if (score > bestScore)
		{
			bestScore = score;
			bestAction = action;
		}
	}
	return bestAction;
}



int Player_Minimax::ScoreTerminalState(const Game& game) const
{
	auto outcome = game.GetPlayState();
	int turnNumber = game.GetTurnNumber();
	assert(outcome != Game::Unfinished);
	assert(turnNumber < 1000 && (turnNumber > 4));

	//tie scores 0
	if (outcome == Game::Tie) return 0;

	//win scores positively with greater scores for earlier wins
	if (outcome == GetPlayerId()) return 1000 - turnNumber;

	//loss scores negatively with greater scores (less negative) for later losses
	return turnNumber - 1000;
}

//score a possibly non-terminal state
int Player_Minimax::ScoreState(Game& game) const
{
	if (game.GetPlayState() == Game::Unfinished) { return 0; }
	return ScoreTerminalState(game);
}


int Player_Minimax::MinmaxState(Game& game, int depth)
{
	if ((depth == 0) || (game.GetPlayState() != Game::Unfinished)) { return ScoreState(game); }
	
	//this players turn
	if (game.GetActivePlayer() == GetPlayerId())
	{
		//maximise
		int bestScore = static_cast<int>(-1e9);

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		
		//for (int action : game.GetValidActions())
		{
			game.Do(action);
			int score = MinmaxState(game, depth - 1);
			game.Undo(action);

			if (score > bestScore) 
			{
				bestScore = score; 
			}
		}
		return bestScore;

	}
	else
		//opponents turn
	{
		//minimise
		int bestScore = static_cast<int>(1e9);

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		//for (int action : game.GetValidActions())
		{
			game.Do(action);
			int score = MinmaxState(game, depth - 1);
			game.Undo(action);

			if (score < bestScore)
			{
				bestScore = score;
			}
		}
		return bestScore;
	}
}


