
#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../Game-Playing/Noughts_and_Crosses.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Random.h"
#include "../Game-Playing/Player_Minimax.h"
#include "../Game-Playing/Player_Alphabeta.h"
#include "../Game-Playing/Player_BestNodeSearch.h"
#include "../Game-Playing/Player_MinmaxWMem.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace NoughtsAndCrosses_Scenarios
{
	using GameClass = Noughts_and_Crosses;

	namespace Scenarios
	{
		using action_t = int;
		using scenario_t = std::vector<action_t>;

		// Board layout
		//  0 1 2
		//  3 4 5
		//  6 7 8

		// scenarios are described by the actions required to set them up
		scenario_t win_at_4   = { 0, 6, 8, 2 };
		//  player 1 should play the winning move, 4
		//  1 _ 2
		//  _[_]_
		//  2 _ 1
	
		scenario_t win_at_6   = { 0, 2, 3, 5 };
		scenario_t win_at_8   = { 0, 2, 1, 5, 6 };
		scenario_t block_at_8 = { 0, 2, 1, 5 };
	
	}


	int ScenarioChoice(auto scenario, auto& player)
	{
		//prepare scenario
		auto game = GameClass();
		for (auto preparationAction : scenario)
		{
			game.Do(preparationAction);
		}
		player.SetPlayerId(game.GetActivePlayer());
		Logger::WriteMessage("scenario:");
		Logger::WriteMessage(game.GetDisplayString().c_str());

		//choose action
		auto chosenAction = player.ChooseAction(game);
		game.Do(chosenAction);
		Logger::WriteMessage("chosen action:");
		Logger::WriteMessage(game.GetDisplayString().c_str());

		return chosenAction;
	}



	TEST_CLASS(Test_minmax)
	{
		using playerClass = Player_Minimax;

		void CheckChoice(auto scenario, auto desiredAction)
		{
			auto player = playerClass();
			auto chosenAction = ScenarioChoice(scenario, player);
			Assert::IsTrue(chosenAction == desiredAction);
		}

		TEST_METHOD(Test_win_at_4){ CheckChoice(Scenarios::win_at_4, 4); }
		TEST_METHOD(Test_win_at_6){ CheckChoice(Scenarios::win_at_6, 6); }
		TEST_METHOD(Test_win_at_8){ CheckChoice(Scenarios::win_at_8, 8); }
		TEST_METHOD(Test_block_at_8){ CheckChoice(Scenarios::block_at_8, 8); }

	};

	TEST_CLASS(Test_alphabeta)
	{
		using playerClass = Player_Alphabeta;

		void CheckChoice(auto scenario, auto desiredAction)
		{
			auto player = playerClass();
			auto chosenAction = ScenarioChoice(scenario, player);
			Assert::IsTrue(chosenAction == desiredAction);
		}

		TEST_METHOD(Test_win_at_4) { CheckChoice(Scenarios::win_at_4, 4); }
		TEST_METHOD(Test_win_at_6) { CheckChoice(Scenarios::win_at_6, 6); }
		TEST_METHOD(Test_win_at_8) { CheckChoice(Scenarios::win_at_8, 8); }
		TEST_METHOD(Test_block_at_8) { CheckChoice(Scenarios::block_at_8, 8); }

	};

	TEST_CLASS(Test_bestNodeSearch)
	{
		using playerClass = Player_BestNodeSearch;

		void CheckChoice(auto scenario, auto desiredAction)
		{
			auto player = playerClass();
			auto chosenAction = ScenarioChoice(scenario, player);
			Assert::IsTrue(chosenAction == desiredAction);
		}

		TEST_METHOD(Test_win_at_4) { CheckChoice(Scenarios::win_at_4, 4); }
		TEST_METHOD(Test_win_at_6) { CheckChoice(Scenarios::win_at_6, 6); }
		TEST_METHOD(Test_win_at_8) { CheckChoice(Scenarios::win_at_8, 8); }
		TEST_METHOD(Test_block_at_8) { CheckChoice(Scenarios::block_at_8, 8); }

	};

	TEST_CLASS(Test_MinmaxWMem)
	{
		using playerClass = Player_MinmaxWMem;

		void CheckChoice(auto scenario, auto desiredAction)
		{
			auto player = playerClass();
			auto chosenAction = ScenarioChoice(scenario, player);
			Assert::IsTrue(chosenAction == desiredAction);
		}

		TEST_METHOD(Test_win_at_4) { CheckChoice(Scenarios::win_at_4, 4); }
		TEST_METHOD(Test_win_at_6) { CheckChoice(Scenarios::win_at_6, 6); }
		TEST_METHOD(Test_win_at_8) { CheckChoice(Scenarios::win_at_8, 8); }
		TEST_METHOD(Test_block_at_8) { CheckChoice(Scenarios::block_at_8, 8); }

	};


}



namespace UnitTestGamePlaying
{

	TEST_CLASS(UnitTest_Minmmax)
	{
	public:

		TEST_METHOD(TestMinmaxVsMinmax)
		{
			Logger::WriteMessage("minmax vs minmax\n");

			int depth = 9;
			Logger::WriteMessage("using depth limit of 2\n");

			auto game = Noughts_and_Crosses();
			auto player1 = Player_Minimax();
			auto player2 = Player_Minimax();
			player1.SetDepthLimit(depth);
			player2.SetDepthLimit(depth);
			auto contest = Contest();

			auto result = contest.PlayGame(game, player1, player2);
			if (result == Game::Tie)
			{
				Logger::WriteMessage("The game was a tie (as expected)\n");
			}
			else
			{
				Logger::WriteMessage("NOT a tie\n");
				Logger::WriteMessage(game.GetDisplayString().c_str());
			}
			Logger::WriteMessage("The game:\n");
			Logger::WriteMessage(game.GetDisplayActionSequenceString().c_str());

			Assert::IsTrue(result == Game::Tie);
		}

	};

	TEST_CLASS(Test_Alphabeta)
	{
	public:

		TEST_METHOD(TestAlphabetaVsAlphabeta)
		{
			Logger::WriteMessage("Alphabeta vs Alphabeta\n");

			int depth = 9;
			Logger::WriteMessage("using depth limit of 9\n");

			auto game = Noughts_and_Crosses();
			auto player1 = Player_Alphabeta();
			auto player2 = Player_Alphabeta();
			player1.SetDepthLimit(depth);
			player2.SetDepthLimit(depth);
			auto contest = Contest();

			auto result = contest.PlayGame(game, player1, player2);
			if (result == Game::Tie)
			{
				Logger::WriteMessage("The game was a tie (as expected)\n");
			}
			else
			{
				Logger::WriteMessage("NOT a tie\n");
			}
			Logger::WriteMessage("The game:\n");
			Logger::WriteMessage(game.GetDisplayActionSequenceString().c_str());

			Assert::IsTrue(result == Game::Tie);
		}
		TEST_METHOD(TestFullDepthNoLoss)
		{
			Logger::WriteMessage("play 10 games of random vs alphabeta depth 9\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_Alphabeta();
			playerAlphabeta.SetDepthLimit(9);
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 10, counts);

			Logger::WriteMessage("checked for no alphabeta losses");
			Assert::IsTrue(counts[Game::Player1Wins] == 0);
		}

		TEST_METHOD(TestDepth3BetterThanRandom)
		{
			Logger::WriteMessage("play 100 games of random vs alhabeta depth 3\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_Alphabeta();
			playerAlphabeta.SetDepthLimit(3);
			auto contest = Contest();
			std::map<Game::PlayState, int> countsAB, countsRand;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 100, countsAB);
			contest.PlayNGames(game, playerRandom, playerRandom, 100, countsRand);

			Logger::WriteMessage("checked for fewer alphabeta losses than a random player in its place");
			Assert::IsTrue(countsAB[Game::Player1Wins] < countsRand[Game::Player1Wins]);
		}

	};

	TEST_CLASS(Test_BestNodeSearch)
	{

	public:
		TEST_METHOD(Test_BestNodeSearch_Vs_BestNodeSearch)
		{
			Logger::WriteMessage("BestNodeSearch vs BestNodeSearch\n");

			int depth = 10;
			Logger::WriteMessage("using full depth\n");

			auto game = Noughts_and_Crosses();
			auto player1 = Player_BestNodeSearch();
			auto player2 = Player_BestNodeSearch();
			player1.SetDepthLimit(depth);
			player2.SetDepthLimit(depth);
			auto contest = Contest();

			auto result = contest.PlayGame(game, player1, player2);
			if (result == Game::Tie)
			{
				Logger::WriteMessage("The game was a tie (as expected)\n");
			}
			else
			{
				Logger::WriteMessage("NOT a tie\n");
			}
			Logger::WriteMessage("The game:\n");
			Logger::WriteMessage(game.GetDisplayActionSequenceString().c_str());

			Assert::IsTrue(result == Game::Tie);
		}

		TEST_METHOD(TestFullDepthNoLoss)
		{
			Logger::WriteMessage("play 10 games of random vs BestNodeSearch depth 9\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerBestNodeSearch = Player_BestNodeSearch();
			playerBestNodeSearch.SetDepthLimit(10);
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, playerRandom, playerBestNodeSearch, 10, counts);

			Logger::WriteMessage("checked for no BestNodeSearch losses");
			Assert::IsTrue(counts[Game::Player1Wins] == 0);
		}

		TEST_METHOD(TestDepth3BetterThanRandom)
		{
			Logger::WriteMessage("play 100 games of random vs BestNodeSearch depth 3\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerBestNodeSearch = Player_BestNodeSearch();
			playerBestNodeSearch.SetDepthLimit(3);
			auto contest = Contest();
			std::map<Game::PlayState, int> countsAB, countsRand;
			contest.PlayNGames(game, playerRandom, playerBestNodeSearch, 100, countsAB);
			contest.PlayNGames(game, playerRandom, playerRandom, 100, countsRand);

			Logger::WriteMessage("checked for fewer BestNodeSearch losses than a random player in its place");
			Assert::IsTrue(countsAB[Game::Player1Wins] < countsRand[Game::Player1Wins]);
		}

	};

}