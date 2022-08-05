
#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../Game-Playing/Noughts_and_Crosses.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Random.h"
#include "../Game-Playing/Player_Minimax.h"
#include "../Game-Playing/Player_Alphabeta.h"
#include "../Game-Playing/Player_BestNodeSearch.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{

	TEST_CLASS(UnitTest_Minmmax)
	{
	public:
		TEST_METHOD(TestPicksWinningMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			//  should play win at 4
			//  1 _ 2
			//  _[_]_
			//  2 _ 1

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 6, 8, 2})
			{
				game.Act(action);
			}

			auto player = Player_Minimax();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 4;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksWinningMove2)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should play winning move at 6
			//  1 _ 2
			//  1 _ 2
			// [_]_ _
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 3, 5})
			{
				game.Act(action);
			}
			auto player = Player_Minimax();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 6;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksBlockingMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should block at cell 8
			//  1 1 2
			//  _ _ 2
			//  _ _[_]
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5})
			{
				game.Act(action);
			}
			Logger::WriteMessage("scenario:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			auto player = Player_Minimax();
			player.SetPlayerId(game.GetActivePlayer());
			int chosenAction = player.ChooseAction(game);

			game.Act(chosenAction);
			Logger::WriteMessage("chosen action:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			int blockingMove = 8;
			Assert::IsTrue(chosenAction == blockingMove);

		}
		TEST_METHOD(TestPicksWinningMove3)
		{

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 _[_]

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5, 6})
			{
				game.Act(action);
			}

			auto player = Player_Minimax();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 8;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


			game.Act(7);
			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 2[_]
			player.SetPlayerId(game.GetActivePlayer());
			winningMove = 3;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


		}
		TEST_METHOD(TestMinmaxVsMinmax)
		{
			Logger::WriteMessage("minmax vs minmax\n");

			int depth = 4;
			Logger::WriteMessage("using depth limit of 4\n");

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
			}
			Logger::WriteMessage("The game:\n");
			Logger::WriteMessage(game.GetDisplayActionSequenceString().c_str());

			Assert::IsTrue(result == Game::Tie);
		}

	};

	TEST_CLASS(Test_Alphabeta)
	{
	public:
		TEST_METHOD(TestPicksWinningMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			//  should play win at 4
			//  1 _ 2
			//  _[_]_
			//  2 _ 1

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 6, 8, 2})
			{
				game.Act(action);
			}

			auto player = Player_Alphabeta();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 4;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksWinningMove2)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should play winning move at 6
			//  1 _ 2
			//  1 _ 2
			// [_]_ _
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 3, 5})
			{
				game.Act(action);
			}
			auto player = Player_Alphabeta();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 6;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksBlockingMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should block at cell 8
			//  1 1 2
			//  _ _ 2
			//  _ _[_]
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5})
			{
				game.Act(action);
			}
			Logger::WriteMessage("scenario:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			auto player = Player_Alphabeta();
			player.SetPlayerId(game.GetActivePlayer());
			int chosenAction = player.ChooseAction(game);

			game.Act(chosenAction);
			Logger::WriteMessage("chosen action:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			int blockingMove = 8;
			Assert::IsTrue(chosenAction == blockingMove);

		}
		TEST_METHOD(TestPicksWinningMove3)
		{

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 _[_]

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5, 6})
			{
				game.Act(action);
			}

			auto player = Player_Alphabeta();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 8;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


			game.Act(7);
			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 2[_]
			player.SetPlayerId(game.GetActivePlayer());
			winningMove = 3;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


		}
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
			Logger::WriteMessage("play 10 games of random vs alhabeta depth 9\n");
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
		TEST_METHOD(TestDepth2BetterThanRandom)
		{
			Logger::WriteMessage("play 100 games of random vs alhabeta depth 2\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_Alphabeta();
			playerAlphabeta.SetDepthLimit(2);
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
		TEST_METHOD(TestPicksWinningMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			//  should play win at 4
			//  1 _ 2
			//  _[_]_
			//  2 _ 1

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 6, 8, 2})
			{
				game.Act(action);
			}

			auto player = Player_BestNodeSearch();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 4;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksWinningMove2)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should play winning move at 6
			//  1 _ 2
			//  1 _ 2
			// [_]_ _
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 3, 5})
			{
				game.Act(action);
			}
			auto player = Player_BestNodeSearch();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 6;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		TEST_METHOD(TestPicksBlockingMove)
		{
			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 1 should block at cell 8
			//  1 1 2
			//  _ _ 2
			//  _ _[_]
			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5})
			{
				game.Act(action);
			}
			Logger::WriteMessage("scenario:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			auto player = Player_BestNodeSearch();
			player.SetPlayerId(game.GetActivePlayer());
			int chosenAction = player.ChooseAction(game);

			game.Act(chosenAction);
			Logger::WriteMessage("chosen action:\n");
			Logger::WriteMessage(game.GetDisplayString().c_str());

			int blockingMove = 8;
			Assert::IsTrue(chosenAction == blockingMove);

		}
		TEST_METHOD(TestPicksWinningMove3)
		{

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 _[_]

			auto game = Noughts_and_Crosses();
			game.Reset();
			for (int action : {0, 2, 1, 5, 6})
			{
				game.Act(action);
			}

			auto player = Player_BestNodeSearch();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 8;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


			game.Act(7);
			// player 2 should win at cell 8
			//  1 1 2
			//  _ _ 2
			//  1 2[_]
			player.SetPlayerId(game.GetActivePlayer());
			winningMove = 3;
			Assert::IsTrue(player.ChooseAction(game) == winningMove);


		}
		TEST_METHOD(TestBestNodeSearchVsBestNodeSearch)
		{
			Logger::WriteMessage("BestNodeSearch vs BestNodeSearch\n");

			int depth = 9;
			Logger::WriteMessage("using depth limit of 9\n");

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
			Logger::WriteMessage("play 10 games of random vs alhabeta depth 9\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_BestNodeSearch();
			playerAlphabeta.SetDepthLimit(9);
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 10, counts);

			Logger::WriteMessage("checked for no BestNodeSearch losses");
			Assert::IsTrue(counts[Game::Player1Wins] == 0);
		}
		TEST_METHOD(TestDepth2BetterThanRandom)
		{
			Logger::WriteMessage("play 100 games of random vs alhabeta depth 2\n");
			auto game = Noughts_and_Crosses();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_BestNodeSearch();
			playerAlphabeta.SetDepthLimit(2);
			auto contest = Contest();
			std::map<Game::PlayState, int> countsAB, countsRand;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 100, countsAB);
			contest.PlayNGames(game, playerRandom, playerRandom, 100, countsRand);

			Logger::WriteMessage("checked for fewer BestNodeSearch losses than a random player in its place");
			Assert::IsTrue(countsAB[Game::Player1Wins] < countsRand[Game::Player1Wins]);
		}

		TEST_METHOD(TestIntDivision)
		{
			//int NextTestScore(int a, int b, int numOptions)
			{
				int a = -1000;
				int b = -2;
				int n = 20;


				double aa = (double)a;
				double bb = (double)b;
				double nn = (double)n;

			/*	int result = a  + ( b - a ) * (n  - 1 ) / n  ;
				double fl = (aa + (bb - aa) * (nn - 1.) / nn);*/
				
				int result = a  + ( b - a ) /2;
				double fl = (aa + (bb - aa) /2.);
				int expected = (int)fl;
				Assert::IsTrue(result == expected);
				//return a + (b - a) * (numOptions - 1) / numOptions;
			}
		}

	};

}