#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../Game-Playing/Game_Connect4.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Alphabeta.h"
#include "../Game-Playing/Player_Random.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{
	TEST_CLASS(UnitTest_Connect4)
	{
	public:

		TEST_METHOD(TestStartsUnfinished)
		{
			auto game = Game_Connect4();
			game.Reset();
			bool bStartsUnfinished = game.GetPlayState() == Game::Unfinished;
			Assert::IsTrue(bStartsUnfinished);
		}

		TEST_METHOD(TestStartsWithValidActions)
		{
			auto game = Game_Connect4();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			bool bHasActions = validActions.size() > 0;
			Assert::IsTrue(bHasActions);
		}
		
		TEST_METHOD(TestTakeAValidAction)
		{
			auto game = Game_Connect4();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			auto action = validActions[0];

			game.Act(action);
		}

		TEST_METHOD(TestPlayer1Starts)
		{
			auto game = Game_Connect4();
			game.Reset();

			int firstActivePlayer = game.GetActivePlayer();
			Assert::IsTrue(firstActivePlayer == 1);
		}

		TEST_METHOD(TestActivePlayer2After1Move)
		{
			auto game = Game_Connect4();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			game.Act(validActions[0]);

			Assert::IsTrue(game.GetActivePlayer() == 2);
		}

		TEST_METHOD(TestP1Win)
		{
			auto game = Game_Connect4();
			game.Reset();

			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  2 _ _ _ _ _ _
			//	  2 1 1 1 1 _ 2

			for (int action : {3, 0, 2, 0, 1, 6, 4})
			{
				game.Act(action);
			}

			Logger::WriteMessage(game.GetDisplayActionSequenceString().c_str());
			Assert::IsTrue(game.GetPlayState() == Game::Player1Wins);

		}

		
		TEST_METHOD(TestP2Win)
		{
			auto game = Game_Connect4();
			game.Reset();

			// p2 wins
			//  _ _ _ 2 _ _ _
			//  _ _ _ 2 _ _ _
			//  _ _ 1 2 _ _ _
			//  1 1 1 2 _ _ _
			for (int action : {0, 3, 2, 3, 2, 3,1,3})
			{
				game.Act(action);
			}

			Assert::IsTrue(game.GetPlayState() == Game::Player2Wins);

		}

		TEST_METHOD(TestWinOnDiagonal)
		{
			auto game = Game_Connect4();
			game.Reset();

			// p1 wins
			//  _ _ _ _ 1 _ _
			//  _ _ _ 1 2 _ _
			//  _ _ 1 2 2 _ _
			//  1 1 1 2 2 _ _
			for (int action : {0, 3, 1, 3, 2, 4, 2, 4,3,4,4})
			{
				game.Act(action);
			}

			Assert::IsTrue(game.GetPlayState() == Game::Player1Wins);

		}

		TEST_METHOD(TestTie)
		{
			auto game = Game_Connect4();
			game.Reset();


			//  tie
			//  2 1 2 1 2 1 2
			//  2 2 1 2 1 2 1
			//  1 2 1 2 1 2 1
			//  1 2 1 2 1 2 1
			//  2 1 2 1 2 1 2
			//  1 2 1 2 1 2 1
			try
			{
				for (int action : {0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6,
					1, 0, 3, 2, 5, 4, 0, 6, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6 })
				{
					game.Act(action);
				}
			} 
			catch (const char* msg)
			{
				Logger::WriteMessage(msg);
				Assert::IsTrue(false);
			}

			Logger::WriteMessage(game.GetDisplayString().c_str());
			Assert::IsTrue(game.GetPlayState() == Game::Tie);

		}
		

	};

	
	TEST_CLASS(Test_Alphabeta)
	{
	public:
		TEST_METHOD(TestPicksWinningMove)
		{
			// Board layout
			//  should play win at 4
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  2 _ _ _ _ _ _
			//	  2 1 1 1 [_] _ 2
			

			auto game = Game_Connect4();
			game.Reset();
			for (int action : {3, 0, 2, 0, 1, 6})
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
			// player 1 should play winning move at 2
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _[_]_ _ _ _
			//	  _ _ 1 _ _ _ _
			//	  2 _ 1 _ _ _ _
			//	  2 1 1 2 _ _ 2
			
			auto game = Game_Connect4();
			game.Reset();
			for (int action : {1, 0, 2, 0, 2,3,2,6})
			{
				game.Act(action);
			}
			auto player = Player_Alphabeta();
			player.SetPlayerId(game.GetActivePlayer());

			int winningMove = 2;
			//Logger::WriteMessage(game.GetDisplayString().c_str());
			Assert::IsTrue(player.ChooseAction(game) == winningMove);

		}
		
		TEST_METHOD(TestPicksBlockingMove)
		{
			
			// player 1 should block at cell 4
			//	  _ _ _ _ _ _ _
			//	  _ _ _ _ _ _ _
			//	  _ _ 2 _ _ _ _
			//	  _ _ 1 2 _ _ _
			//	  2 _ 1 1[_]_ _
			//	  2 1 1 2 1 2 _
			
			auto game = Game_Connect4();
			game.Reset();
			for (int action : {1, 0, 2, 0, 2,3,2,5,3,3,4,2})
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

			int blockingMove = 4;
			Assert::IsTrue(chosenAction == blockingMove);

		}
		
		TEST_METHOD(TestRandomVsAlphabeta)
		{
			Logger::WriteMessage("play 100 games of random vs alphabeta depth 2\n");

			int depth = 2;

			auto game = Game_Connect4();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_Alphabeta();
			playerAlphabeta.SetDepthLimit(depth);
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 100, counts);

			Logger::WriteMessage("checked for no alphabeta losses");
			Assert::IsTrue(counts[Game::Player1Wins] == 0);
		}
		
		
	};


}
