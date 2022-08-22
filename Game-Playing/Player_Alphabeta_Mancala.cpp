#include "Player_Alphabeta_Mancala.h"
#include "Game_Mancala.h"
#include <assert.h>



// an alphabeta-based player for mancala

//optimisations:
//looping over all actions and skipping the invalid ones is faster than getting a vector of valid actions (was approx 30% faster)
//



/// <summary>
/// Choose an action to win as early as possible or lose as late as possible.
/// </summary>
/// <param name="game">: reference to the current game state</param>
/// <returns></returns>
int Player_Alphabeta_Mancala::ChooseAction(const GameClass& game)
{

	//choose an action to maximise minimum score

	int bestAction = -1;
	int bestScore = static_cast<int>(-1e9);
	int a = static_cast<int>(-1e9);
	int b = static_cast<int>(1e9);

	std::vector<int> validActions;
	game.GetValidActions(validActions);
	for (int action : validActions)
	{
		auto nextState = GameClass(game);
		nextState.Do(action);
		int score = MinmaxState(nextState, depthLimit, a, b);

		if (score > a)
		{
			// acceptable score above 'a'
			// since we are maximising, any further acceptable actions will also score above 'a'
			// improved lower bound
			a = score;
		}

		if (score > bestScore)
		{
			bestScore = score;
			bestAction = action;
		}


	}

	return bestAction;
}


//// assign greater scores for more favorable outcomes
//// win as early as possible or tie or lose as late as possible
//int Player_Alphabeta_Mancala::ScoreTerminalState(const GameClass& game) const
//{
//	auto outcome = game.GetPlayState();
//	int turnNumber = game.GetTurnNumber();
//	assert(outcome != Game::Unfinished);
//	assert(turnNumber < 1000 && (turnNumber > 4));
//
//	// tie scores 0
//	if (outcome == Game::Tie) return 0;
//
//	// win scores positively with greater scores for earlier wins
//	if (outcome == GetPlayerId()) return 1000 - turnNumber;
//
//	// loss scores negatively with greater scores (less negative) for later losses
//	return turnNumber - 1000;
//}
//
//
//int Player_Alphabeta_Mancala::ScoreLowerBound(const GameClass& game) const
//{
//	// score of losing next turn
//	return (game.GetTurnNumber() + 1) - 1000;
//}
//
//
//int Player_Alphabeta_Mancala::ScoreUpperBound(const GameClass& game) const
//{
//	// score of winning 2 turns later
//	return 1000 - (game.GetTurnNumber() + 2);
//}
//
//
////score a possibly non-terminal state
//int Player_Alphabeta_Mancala::ScoreState(GameClass& game) const
//{
//	//PlayoutState(game);
//	if (game.GetPlayState() == Game::Unfinished) { return 0; }
//	return ScoreTerminalState(game);
//}



//score a state based on how many beads in your mancala - how many in your opponents mancala
//(does not care about winning early/losing late)

//score a possibly non-terminal state
int Player_Alphabeta_Mancala::ScoreState(GameClass& game) const
{
	if (GetPlayerId() == game.GetActivePlayer())
	{
		return game.ActivePlayerAdvantage();
	}
	else
	{
		return -game.ActivePlayerAdvantage();
	}
}


int Player_Alphabeta_Mancala::ScoreTerminalState(const GameClass& game) const
{
	assert(false);
	return 0;
}


int Player_Alphabeta_Mancala::ScoreLowerBound(const GameClass& game) const
{
	int wonBeads = GetPlayerId() == game.GetActivePlayer() ? game.ActivePlayerScore() : game.OpponentScore();
	int otherBeads = 48 - wonBeads;
	return wonBeads - otherBeads;
}


int Player_Alphabeta_Mancala::ScoreUpperBound(const GameClass& game) const
{
	int lostBeads = GetPlayerId() == game.GetActivePlayer() ? game.OpponentScore() : game.ActivePlayerScore();
	int otherBeads = 48 - lostBeads;
	return otherBeads - lostBeads;
}





/// <summary>
/// Calculate minmax score of a game state using AlphaBeta pruning.
/// Only determine precise scores between 'a' and 'b'.
/// Uses fail-soft AlphaBeta; it can return an upper bound less than 'a' or a lower bound greater than 'b'.
/// 
/// </summary>
/// <param name="game">: The game state to evaluate</param>
/// <param name="depth">: How many moves to look ahead beyond the immediate choice</param>
/// <param name="a">: The lower bound on relevant scores</param>
/// <param name="b">: The upper bound on relevant scores</param>
/// <returns></returns>
int Player_Alphabeta_Mancala::MinmaxState(GameClass& game, int depth, int a, int b)
{
	if ((depth == 0) || (game.GetPlayState() != Game::Unfinished)) { return ScoreState(game); }


	//this players turn
	if (game.GetActivePlayer() == GetPlayerId())
	{
		// maximise score
		// using a bestScore seperate from 'a' allows retuning an upper bound below 'a'
		int bestScore = ScoreLowerBound(game);
		if (bestScore >= b)
		{
			return bestScore;
		}

		//std::vector<int> validActions;
		//game.GetValidActions(validActions);

		//for (int action : game.GetValidActions())
		//{
		for (int action : game.AllActions)
		{
			if (!game.IsValidAction(action)) { continue; }

			auto nextState = GameClass(game);
			nextState.Do(action);
			int score = MinmaxState(nextState, depth - 1, a, b);

			/*game.Do(action);
			int score = MinmaxState(game, depth - 1, a, b);
			game.Undo(action);*/

			if (score >= b)
			{
				// acceptable score >= 'b'
				// since we are maximising, any further acceptable moves will also be >= 'b'
				// we have a lower bound >= 'b'
				// we dont need to try the other moves
				return score;
			}

			if (score > a)
			{
				// acceptable score above 'a'
				// since we are maximising, any further acceptable moves will also be above 'a'
				// improved lower bound
				a = score;
				// note that if we were below 'a', we may yet see a score above 'a' so we can not be sure we are outside the bounds
			}

			if (score > bestScore)
			{
				// find the biggest scoring move
				bestScore = score;
			}



		}
		// best score might be less than a 'a' (i.e. if all possible scores were less than 'a')
		return bestScore;

	}
	else
		//opponents turn
	{
		//minimise
		//using a bestScore seperate from 'b' allows retuning an lower bound above 'b'

		//initally worst score
		int bestScore = ScoreUpperBound(game);
		if (bestScore <= a)
		{
			// we already have a sufficient upper bound
			return bestScore;
		}

		//std::vector<int> validActions;
		//game.GetValidActions(validActions);

		//for (int action : game.GetValidActions())
		//{
		for (int action : game.AllActions)
		{
			if (!game.IsValidAction(action)) { continue; }

			auto nextState = GameClass(game);
			nextState.Do(action);
			int score = MinmaxState(nextState, depth - 1, a, b);

			/*game.Do(action);
			int score = MinmaxState(game, depth - 1, a, b);
			game.Undo(action);*/

			if (score <= a)
			{
				// acceptable score <= 'a'
				// since we are minimising, any further acceptable moves will also be <= 'a'
				// we have an upper bound <= 'a'
				// we dont need to try the other moves
				return score;
			}

			if (score < b)
			{
				// acceptable score below 'b'
				// since we are minimising, any further acceptable moves will also be below 'b'
				// improved upper bound
				b = score;
				// note that if we were above 'b', we may yet see a score below 'b' so we can not be sure we are outside the bounds a-b
			}


			if (score < bestScore)
			{
				// find the lowest scoring move
				bestScore = score;
			}
		}
		// best score might be greater than 'b' (i.e. if all possible scores were greater than 'b')
		return bestScore;
	}

	assert(false);
	return 0;
}