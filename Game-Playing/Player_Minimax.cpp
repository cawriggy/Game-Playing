#include "Player_Minimax.h"
#include <assert.h>
#include <cstdlib> //for rand


int Player_Minimax::ChooseAction(const Game& game)
{
	//choose an action to maximise minimum score
	int bestScore = static_cast<int>(- 1e9);
	int bestAction = -1;

	std::vector<int> validActions;
	game.GetValidActions(validActions);
	for (int action : validActions)
	{
		auto nextState = game.Clone();
		nextState->Act(action);
		int score = MinmaxState(*nextState, GetDepthLimit());
		
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
	PlayoutState(game);
	return ScoreTerminalState(game);
}

//use playout policy to reach a terminal state
void Player_Minimax::PlayoutState(Game& game) const
{
	while (game.GetPlayState() == Game::Unfinished)
	{
		game.Act(PlayoutPolicy(game));
	}
}

//how to choose a move when playing to a terminal state to get a score
int Player_Minimax::PlayoutPolicy(const Game& game) const
{
	//pick first valid move
	/*std::vector<int> validActions;
	game.GetValidActions(validActions);
	return validActions[0];*/

	//pick random move
	std::vector<int> validActions;
	game.GetValidActions(validActions);
	return validActions[rand() % validActions.size()];
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
		{
			auto nextState = game.Clone();
			nextState->Act(action);
			int score = MinmaxState(*nextState, depth - 1);
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
		{
			auto nextState = game.Clone();
			nextState->Act(action);
			int score = MinmaxState(*nextState, depth - 1);
			if (score < bestScore)
			{
				bestScore = score;
			}
		}
		return bestScore;
	}

	assert(false);
	return 0;
}


