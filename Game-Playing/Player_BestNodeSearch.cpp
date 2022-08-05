// implement a best node search
// 
// we have a game state and depending on our choice of action we could end up at several different game states
// we want to pick the option with the greatest minmax score
// we don't require the minmax score we just want the best choice



#include "Player_BestNodeSearch.h"
#include <assert.h>
#include <cstdlib> //for rand (in playout policy)
#include <vector>
#include <map>


/// <summary>
/// Choose an action to win as early as possible or lose as late as possible.
/// Uses alphabeta pruning to minmax more efficiently.
/// </summary>
/// <param name="game">: reference to the current game state</param>
/// <returns></returns>
int Player_BestNodeSearch::ChooseAction(const Game& game)
{
	//choose an action to maximise minimum score

	//int bestAction = -1;
	//int bestScore = static_cast<int>(-1e9);
	//int a = static_cast<int>(-1e9);
	//int b = static_cast<int>(1e9);

	int a = ScoreLowerBound(game) - 1;
	int b = ScoreUpperBound(game) + 1;

	//std::vector<int> validActions;
	//game.GetValidActions(validActions);
	//for (int action : validActions)
	//{
	//	auto nextState = game.Clone();
	//	nextState->Act(action);

	//	int score = MinmaxState(*nextState, depthLimit, a, b);

	//	if (score > a)
	//	{
	//		// acceptable score above 'a'
	//		// since we are maximising, any further acceptable actions will also score above 'a'
	//		// improved lower bound
	//		a = score;
	//	}

	//	if (score > bestScore)
	//	{
	//		bestScore = score;
	//		bestAction = action;
	//	}


	//}

	//return bestAction;

	int action = BestNodeSearch(game, a, b);
	HasDoneInitialSearch = true;
	return action;
}


// assign greater scores for more favorable outcomes
// win as early as possible or tie or lose as late as possible
int Player_BestNodeSearch::ScoreTerminalState(const Game& game) const
{
	auto outcome = game.GetPlayState();
	int turnNumber = game.GetTurnNumber();
	assert(outcome != Game::Unfinished);
	assert(turnNumber < 1000 && (turnNumber > 4));

	// tie scores 0
	if (outcome == Game::Tie) return 0;

	// win scores positively with greater scores for earlier wins
	if (outcome == GetPlayerId()) return 1000 - turnNumber;

	// loss scores negatively with greater scores (less negative) for later losses
	return turnNumber - 1000;
}


int Player_BestNodeSearch::ScoreLowerBound(const Game& game) const
{
	// score of losing next turn
	return (game.GetTurnNumber() + 1) - 1000;
}


int Player_BestNodeSearch::ScoreUpperBound(const Game& game) const
{
	// score of winning 2 turns later
	return 1000 - (game.GetTurnNumber() + 2);
}


//score a possibly non-terminal state
int Player_BestNodeSearch::ScoreState(Game& game) const
{
	PlayoutState(game);
	return ScoreTerminalState(game);
}


//use playout policy to reach a terminal state
void Player_BestNodeSearch::PlayoutState(Game& game) const
{
	while (game.GetPlayState() == Game::Unfinished)
	{
		game.Act(PlayoutPolicy(game));
	}
}


//how to choose a move when playing to a terminal state to get a score
int Player_BestNodeSearch::PlayoutPolicy(const Game& game) const
{
	std::vector<int> validActions;
	game.GetValidActions(validActions);

	//pick random move
	return validActions[rand() % validActions.size()];
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
int Player_BestNodeSearch::MinmaxState(Game& game, int depth, int a, int b)
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

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		{
			auto nextState = game.Clone();
			nextState->Act(action);

			int score = MinmaxState(*nextState, depth - 1, a, b);

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

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		{
			auto nextState = game.Clone();
			nextState->Act(action);

			int score = MinmaxState(*nextState, depth - 1, a, b);

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





int Player_BestNodeSearch::BestNodeSearch(const Game& game, int a, int b)
{
	//todo: check for redundant ifs
	//todo: correct it so it makes all winning moves
	//todo: make it faster
	//todo: start with previous test value

	std::vector<int> validActions;
	game.GetValidActions(validActions);

	int numOptions = validActions.size();
	int bestAction = -1;
	int numBetter;
	int greatestValue = static_cast<int>(-1e9);
	bool first = false;
	int bestValue = static_cast<int>(-1e9);
	
	int start = 0;
	int end = validActions.size() - 1;
	bool atStart = true;

	do
	{
		int test = NextTestScore(a, b, numOptions);
		if (first)
		{
			test = previousScoreTest;
			first = false;
		}

		
		numBetter = 0;

		//for (int action : validActions)

		// keep relevent actions at the start of the vector
		// loop over the relevent actions
		start = 0;
		end = numOptions - 1;
		for (int i = 0; i <= end;i++)
		{
			int action = validActions[i];
			auto nextState = game.Clone();
			nextState->Act(action);

			// perform focused search to check if the state is valued above or below test
			int value = MinmaxState(*nextState, GetDepthLimit(), test-1, test+1);

			if (value >= test)
			{
				numBetter++;
				bestAction = action;

				//if (HasDoneInitialSearch) 
				//{
				//	return action;
				//}

				if (value > bestValue)
				{
					bestValue = value;
				}

				//raise the bar upon finding a better lower bound
				if (value > test)
				{
					test = value;
				}

			}

			if (value < test)
			{
				//no longer consider the action
				//swap the action to the end and reduce options by 1 (end--), (i-- to prevent advancing past the swapped in action this iteration)
				std::swap(validActions[i--], validActions[end--]);
			}

		}

		if (bestValue < test)
		{
			b = bestValue;
		}

		if (numBetter < 1)
		{
			//test value too high
			//nothing passed it
			if (test < b)
			{
				b = test;
			}

		}

		if (numBetter > 1)
		{
			//test too low
			//many things passed it
			if (test > a)
			{
				a = test;
			}
		}

		if (numBetter > 0)
		{
			numOptions = numBetter;
		}


		if (bestValue == test)
		{
			//stop if the best value matches the test
			// if using a unit window (b=a+2) then this means we found the exact minmax value so we are done
			// if using a null window (b=a+1) then there could be a better option (not all the best so far may be truely equal) but this might be unlikely
			//    especially if good move ordering is used.
			break;
		}

		//remember the last test we used so we can start with it next time
		previousScoreTest = test;

	} while ((b - a >= 2) && (numBetter != 1));
	//previousScoreTest = test


	if (bestAction == -1)
	{
		bestAction = validActions[0];
	}

	return bestAction;
}




//
//int Player_BestNodeSearch::BestNodeSearch(const Game& game, int a, int b)
//{
//	//todo: check for redundant ifs
//
//	std::vector<int> validActions;
//	game.GetValidActions(validActions);
//
//	int numOptions = validActions.size();
//	int bestAction = -1;
//	int numBetter;
//	int greatestValue = static_cast<int>(-1e9);
//	do
//	{
//		int test = NextTestScore(a, b, numOptions);
//		numBetter = 0;
//
//		for (int action : validActions)
//		{
//			//get next state
//			auto nextState = game.Clone();
//			nextState->Act(action);
//
//			// perform null window search to check if valued above or below test
//			int value = MinmaxState(*nextState, GetDepthLimit(), test-1, test + 1);
//
//			if (value >= test)
//			{
//				numBetter++;
//				bestAction = action;
//
//				if (value > a)
//				{
//					a = value;
//				}
//
//			}
//
//			if (value > greatestValue)
//			{
//				greatestValue = value;
//			}
//
//		}
//		//checked all actions
//
//		if (greatestValue < b)
//		{
//			b = greatestValue;
//		}
//
//		if (numBetter < 1)
//		{
//			//test value too high
//			if (test < b)
//			{
//				b = test;
//			}
//
//		}
//
//		if (numBetter > 1)
//		{
//			//test too low
//			if (test > a)
//			{
//				a = test;
//			}
//		}
//
//		if (numBetter > 0)
//		{
//			numOptions = numBetter;
//		}
//
//	} while ((b - a >= 2) && (numBetter != 1));
//
//	return bestAction;
//}
