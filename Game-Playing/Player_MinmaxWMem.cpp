#include "Player_MinmaxWMem.h"
#include <assert.h>


int Player_MinmaxWMem::ChooseAction(const Game& game)
{
	////cached scores may be invalid if need greater depth or we switched places (i.e was player 1 now player 2)
	////clearing the entire cache is a simple way to remove invalid elements but also any useful ones
	//scoreCache.clear();

	//choose an action to maximise minimum score
	int bestScore = static_cast<int>(-1e9);
	int bestAction = -1;

	std::vector<int> validActions;
	game.GetValidActions(validActions);
	for (int action : validActions)
	{
		auto nextState = game.Clone();
		nextState->Do(action);
		int score = MinmaxState(*nextState, GetDepthLimit());

		if (score > bestScore)
		{
			bestScore = score;
			bestAction = action;
		}
	}
	return bestAction;
}



int Player_MinmaxWMem::ScoreTerminalState(const Game& game) const
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
int Player_MinmaxWMem::ScoreState(Game& game) const
{
	if (game.GetPlayState() == Game::Unfinished) { return 0; }
	return ScoreTerminalState(game);
}


int Player_MinmaxWMem::MinmaxState(Game& game, int depth)
{
	if ((depth == 0) || (game.GetPlayState() != Game::Unfinished)) { return ScoreState(game); }

	if (scoreCache.contains(game.GetStateVector()))
	{
		return scoreCache[game.GetStateVector()];
	}

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
			nextState->Do(action);
			int score = MinmaxState(*nextState, depth - 1);
			if (score > bestScore)
			{
				bestScore = score;
			}
		}
		scoreCache[game.GetStateVector()] = bestScore;
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
			nextState->Do(action);
			int score = MinmaxState(*nextState, depth - 1);
			if (score < bestScore)
			{
				bestScore = score;
			}
		}
		scoreCache[game.GetStateVector()] = bestScore;
		return bestScore;
	}
}




