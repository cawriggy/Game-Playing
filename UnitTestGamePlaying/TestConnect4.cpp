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
		using GameClass = Game_Connect4;

		int GameOutcome(auto actions)
		{
			auto gameObj = GameClass();
			gameObj.Reset();
			try
			{ 
				for (auto action : actions)
				{
				gameObj.Act(action);
				}
			}
			catch (const char* msg)
			{
				Logger::WriteMessage(msg);
				Assert::IsTrue(false);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			return gameObj.GetPlayState();
		}

		TEST_METHOD(TestStartsUnfinished)
		{
			auto game = GameClass();
			Assert::IsTrue(game.GetPlayState() == Game::Unfinished);
		}

		TEST_METHOD(TestStartsWithValidActions)
		{
			auto game = GameClass();
			std::vector<int> validActions;
			game.GetValidActions(validActions);
			Assert::IsTrue(validActions.size() > 0);
		}

		TEST_METHOD(TestTakeAValidAction)
		{
			auto game = GameClass();
			std::vector<int> validActions;
			game.GetValidActions(validActions);
			Assert::IsTrue(validActions.size() > 0);
			game.Act(validActions[0]);
		}

		TEST_METHOD(TestPlayer1Starts)
		{
			auto game = GameClass();
			Assert::IsTrue(game.GetActivePlayer() == 1);
		}
		
		TEST_METHOD(TestActivePlayer2After1Move)
		{
			auto game = GameClass();
			std::vector<int> validActions;
			game.GetValidActions(validActions);
			game.Act(validActions[0]);
			Assert::IsTrue(game.GetActivePlayer() == 2);
		}

		
		TEST_METHOD(TestP1Win)
		{
			auto actions = { 3, 0, 2, 0, 1, 6, 4 };
			Logger::WriteMessage("Test player 1 wins");
			Assert::IsTrue(GameOutcome(actions) == Game::Player1Wins);
		}

		
		TEST_METHOD(TestP2Win)
		{
			auto actions = { 0, 3, 2, 3, 2, 3,1,3 };
			Logger::WriteMessage("Test player 2 wins");
			Assert::IsTrue(GameOutcome(actions) == Game::Player2Wins);
		}


		TEST_METHOD(TestWinOnDiagonal)
		{
			auto actions = { 0, 3, 1, 3, 2, 4, 2, 4,3,4,4 };
			Logger::WriteMessage("Test player 1 wins on diagonal");
			Assert::IsTrue(GameOutcome(actions) == Game::Player1Wins);
		}

		TEST_METHOD(TestTie)
		{
			auto actions = { 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6,
					1, 0, 3, 2, 5, 4, 0, 6, 1, 2, 3, 4, 5, 6, 0, 1, 2, 3, 4, 5, 6 };
			Logger::WriteMessage("Test a Tie");
			Assert::IsTrue(GameOutcome(actions) == Game::Tie);

		}
		

	};


	
	TEST_CLASS(Test_Connect4_Alphabeta)
	{
	public:
		using GameClass = Game_Connect4;

		int PlayerChoice(auto actions)
		{
			auto gameObj = GameClass();
			gameObj.Reset();
			try
			{
				for (auto action : actions)
				{
					gameObj.Act(action);
				}
			}
			catch (const char* msg)
			{
				Logger::WriteMessage(msg);
				Assert::IsTrue(false);
			}

			auto player = Player_Alphabeta();
			player.SetPlayerId(gameObj.GetActivePlayer());

			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			return player.ChooseAction(gameObj);
		}

		TEST_METHOD(TestPicksWinningMove)
		{
			auto actions = { 3, 0, 2, 0, 1, 6 };
			int winningMove = 4;
			Logger::WriteMessage("Test player picks wining move : 4");
			Assert::IsTrue(PlayerChoice(actions) == winningMove);
		}
		
		TEST_METHOD(TestPicksWinningMove2)
		{
			auto actions = { 1, 0, 2, 0, 2,3,2,6 };
			int winningMove = 2;
			Logger::WriteMessage("Test player picks wining move : 2");
			Assert::IsTrue(PlayerChoice(actions) == winningMove);
		}
		
		TEST_METHOD(TestPicksBlockingMove)
		{
			auto actions = { 1, 0, 2, 0, 2,3,2,5,3,3,4,2 };
			int blockingMove = 4;
			Logger::WriteMessage("Test player picks blocking move : 4");
			Assert::IsTrue(PlayerChoice(actions) == blockingMove);
		}
		
		TEST_METHOD(TestRandomVsAlphabeta)
		{
			Logger::WriteMessage("Random player vs Alphabeta player depth 3\n");

			int depth = 3;

			auto game = Game_Connect4();
			auto playerRandom = Player_Random();
			auto playerAlphabeta = Player_Alphabeta();
			playerAlphabeta.SetDepthLimit(depth);
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, playerRandom, playerAlphabeta, 1, counts);

			Logger::WriteMessage("checked for no alphabeta losses");
			Assert::IsTrue(counts[Game::Player1Wins] == 0);
		}
		
		
	};


}
