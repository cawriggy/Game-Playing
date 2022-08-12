// implement a best node search
// 
// we have a game state and depending on our choice of action we could end up at several different game states
// we want to pick the option with the greatest minmax score
// we don't require the minmax score we just want the best choice

//
// ideas
// iterative deepening
// order moves
// 
//


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

	//clear caches, a simple yet suboptimal way to help maintain consistency with a receding horizon
	LowerBoundCache.clear();
	UpperBoundCache.clear();

	int a = ScoreLowerBound(game) - 1;
	int b = ScoreUpperBound(game) + 1;

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
	//PlayoutState(game);
	if (game.GetPlayState() == Game::Unfinished) { return 0; }
	return ScoreTerminalState(game);
}


////use playout policy to reach a terminal state
//void Player_BestNodeSearch::PlayoutState(Game& game) const
//{
//	while (game.GetPlayState() == Game::Unfinished)
//	{
//		game.Do(PlayoutPolicy(game));
//	}
//}
//
//
////how to choose a move when playing to a terminal state to get a score
//int Player_BestNodeSearch::PlayoutPolicy(const Game& game) const
//{
//	std::vector<int> validActions;
//	game.GetValidActions(validActions);
//
//	//pick random move
//	return validActions[rand() % validActions.size()];
//}




/// <summary>
/// Calculate minmax score of a game state using AlphaBeta pruning.
/// Only determine precise scores between a and b.
/// Uses fail-soft AlphaBeta; it can return an upper bound at most a or a lower bound at least b.
/// The exact minmax value will be returned if it lies between a and b.
/// </summary>
/// <param name="game">: the game state to evaluate</param>
/// <param name="depth">: how many moves to look ahead beyond the immediate choice</param>
/// <param name="a">: the lower bound on relevant scores</param>
/// <param name="b">: the upper bound on relevant scores</param>
/// <returns>the minmax value of the game state</returns>
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

		auto stateVector = game.GetStateVector();
		if (LowerBoundCache.contains(stateVector))
		{
			int bound = LowerBoundCache[stateVector];
			if (bound > b)
			{
				return bound;
			}
			else if (bound > a)
			{
				a = bound-1;
			}
		} 

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		{
			auto nextState = game.Clone();
			nextState->Do(action);

			int score = MinmaxState(*nextState, depth - 1, a, b);

			if (score >= b)
			{
				// acceptable score >= 'b'
				// since we are maximising, any further acceptable moves will also be >= 'b'
				// we have a lower bound >= 'b'
				// we dont need to try the other moves
				////LowerBoundCache[stateVector] = score-1;
				////stateVector = nextState->GetStateVector();
				//LowerBoundCache[stateVector] = score-1;
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

		auto stateVector = game.GetStateVector();
		if (UpperBoundCache.contains(stateVector))
		{
			int bound = UpperBoundCache[stateVector];
			if (bound < a)
			{
				return bound;
			}
			else if (bound < b)
			{
				a = bound + 1;
			}
		}

		std::vector<int> validActions;
		game.GetValidActions(validActions);
		for (int action : validActions)
		{
			auto nextState = game.Clone();
			nextState->Do(action);

			int score = MinmaxState(*nextState, depth - 1, a, b);

			if (score <= a)
			{
				// acceptable score <= 'a'
				// since we are minimising, any further acceptable moves will also be <= 'a'
				//// we have an upper bound <= 'a'
				//// we dont need to try the other moves
				//UpperBoundCache[stateVector] = a;
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
	//todo: make it faster

	std::vector<int> validActions;
	game.GetValidActions(validActions);

	int numOptions = validActions.size();
	int bestAction = -1;
	int numBetter = 0;
	int bestValue = static_cast<int>(-1e9);
	
	bool first = true;
	int end = validActions.size() - 1;

	do
	{
		int test = (a + b) / 2; //NextTestScore(a, b, numOptions);
		
		//use the previous test value first
		if (first)
		{
			test = previousScoreTest;
			first = false;
		}
		//store the last test we used so we can start with it next time
		previousScoreTest = test;


		numBetter = 0;

		// loop over the relevent actions while swapping irrelevent actions out of the way
		end = numOptions - 1;
		for (int i = 0; i <= end;i++)
		{
			int action = validActions[i];

			auto nextState = game.Clone();
			nextState->Do(action);
			// perform focused search to check if the state is valued above or below test
			int value = MinmaxState(*nextState, GetDepthLimit(), test-1, test+1);


			if (value >= test)
			{
				numBetter++;
				bestAction = action;
				test = value;

				if (value > bestValue)
				{
					bestValue = value;
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
			//many options passed it
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
			// if using a unit window (b=a+2) then we found the exact minmax value so we are done
			// if using a null window (b=a+1) then there could be a better option but this might be unlikely (not all the best so far may be truely equal)
			break;
		}


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
//	//todo: make it faster, (possibly make it recursive?)
//
//	//std::map<int, int> upperBoundCache;
//	//std::map<int, int> lowerBoundCache;
//	//std::map<int, int> valueCache;
//
//
//	std::vector<int> validActions;
//	game.GetValidActions(validActions);
//
//	int numOptions = validActions.size();
//	int bestAction = -1;
//	int numBetter = 0;
//	//int greatestValue = static_cast<int>(-1e9);
//	int bestValue = static_cast<int>(-1e9);
//
//	bool first = true;
//	int end = validActions.size() - 1;
//
//	do
//	{
//		int test = NextTestScore(a, b, numOptions);
//		if (first)
//		{
//			test = previousScoreTest;
//			first = false;
//		}
//		//store the last test we used so we can start with it next time
//		previousScoreTest = test;
//
//
//		numBetter = 0;
//
//		//for (int action : validActions)
//
//		// loop over the relevent actions while swapping irrelevent actions out of the way
//		end = numOptions - 1;
//		for (int i = 0; i <= end; i++)
//		{
//			int action = validActions[i];
//			auto nextState = game.Clone();
//			nextState->Do(action);
//
//			// perform focused search to check if the state is valued above or below test
//			int value = MinmaxState(*nextState, GetDepthLimit(), test - 1, test + 1);
//
//			if (value >= test)
//			{
//				numBetter++;
//				bestAction = action;
//
//				if (value > bestValue)
//				{
//					bestValue = value;
//				}
//
//				//raise the test upon finding a better lower bound
//				if (value > test)
//				{
//					test = value;
//				}
//
//			}
//
//			if (value < test)
//			{
//				//no longer consider the action
//				//swap the action to the end and reduce options by 1 (end--), (i-- to prevent advancing past the swapped in action this iteration)
//				std::swap(validActions[i--], validActions[end--]);
//			}
//
//		}
//
//		if (bestValue < test)
//		{
//			b = bestValue;
//		}
//
//		if (numBetter < 1)
//		{
//			//test value too high
//			//nothing passed it
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
//			//many things passed it
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
//
//		if (bestValue == test)
//		{
//			//stop if the best value matches the test
//			// if using a unit window (b=a+2) then we found the exact minmax value so we are done
//			// if using a null window (b=a+1) then there could be a better option but this might be unlikely (not all the best so far may be truely equal)
//			break;
//		}
//
//
//	} while ((b - a >= 2) && (numBetter != 1));
//	//previousScoreTest = test
//
//
//	if (bestAction == -1)
//	{
//		bestAction = validActions[0];
//	}
//
//	return bestAction;
//}
