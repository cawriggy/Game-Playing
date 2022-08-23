#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../Game-Playing/Game_Mancala.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Alphabeta.h"
#include "../Game-Playing/Player_Random.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{
	TEST_CLASS(UnitTest_Mancala)
	{
	public:
		using GameClass = Game_Mancala;

		int GameOutcome(auto actions)
		{
			auto gameObj = GameClass();
			try
			{
				for (auto action : actions)
				{
					gameObj.Do(action);
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

		std::vector<int> GameState(auto actions)
		{
			auto gameObj = GameClass();
			try
			{
				for (auto action : actions)
				{
					gameObj.Do(action);
				}
			}
			catch (const char* msg)
			{
				Logger::WriteMessage(msg);
				Assert::IsTrue(false);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			return gameObj.GetStateVector();
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

		TEST_METHOD(TestDisplay)
		{
			auto game = GameClass();
			auto actions = { 0,1 };
			Logger::WriteMessage("Test Display");
			Assert::IsTrue(GameOutcome(actions) == Game::Unfinished);
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
			game.Do(validActions[0]);
			Assert::IsTrue(game.GetActivePlayer() == 2);
		}

		TEST_METHOD(TestDoAction)
		{
			int action = 3;
			auto game = GameClass();
			game.Do(action);
			std::vector<int> expectedNewState = { 5, 4, 4, 4, 4, 4, 0, 4, 4, 4, 0, 5, 5, 1,2 };
			std::vector<int> newState = game.GetStateVector();
			Logger::WriteMessage(game.GetDisplayString().c_str());
			Assert::IsTrue(newState == expectedNewState);
		}


		TEST_METHOD(TestIsValidAction)
		{
			auto actions = { 5, 1 };
			Logger::WriteMessage("Test Invalid Action");
			int invalid_action = 1;
			auto gameObj = GameClass();
			for (auto action : actions)
			{
				gameObj.Do(action);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			Assert::IsTrue(gameObj.IsValidAction(invalid_action) == false);
		}

		TEST_METHOD(TestIsValidAction2)
		{
			auto actions = { 5, 1 };
			Logger::WriteMessage("Test Invalid Action");
			int invalid_action = 6;
			auto gameObj = GameClass();
			for (auto action : actions)
			{
				gameObj.Do(action);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			Assert::IsTrue(gameObj.IsValidAction(invalid_action) == false);
		}

		TEST_METHOD(TestPlayerKeepsTurn)
		{
			auto actions = { 5, 1 };
			Logger::WriteMessage("Test Player 2 keeps turn");
			auto gameObj = GameClass();
			for (auto action : actions)
			{
				gameObj.Do(action);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			Assert::IsTrue(gameObj.GetActivePlayer() == 2);
		}

		TEST_METHOD(TestBeadCapture)
		{
			Logger::WriteMessage("Test Bead Capture");
			auto init_actions = { 5, 1, 0, 0, 2, 1, 3, 2, 0, 0};
			std::vector<int> initState = GameState(init_actions);
			auto actions = { 5, 1, 0, 0, 2, 1, 3, 2, 0, 0, 5};
			Logger::WriteMessage("Play cup 5");
			std::vector<int> newState = GameState(actions);
			std::vector<int> expectedNewState = {1,3,2,9,8,0,4,1,3,2,0,8,0,7, 1};
			Assert::IsTrue(newState == expectedNewState);
		}

		TEST_METHOD(TestRandomVsRandom)
		{
			Logger::WriteMessage("Random player vs Random player\n");

			auto game = GameClass();
			auto player1Random = Player_Random();
			auto player2Random = Player_Random();
			auto contest = Contest();
			std::map<Game::PlayState, int> counts;
			contest.PlayNGames(game, player1Random, player2Random, 100, counts);
			std::string results = "Results: ";
			results += "Wins for 1: " + std::to_string(counts[Game::Player1Wins]);
			results += ", Wins for 2: " + std::to_string(counts[Game::Player2Wins]);
			results += ", Ties: " + std::to_string(counts[Game::Tie]);
			Logger::WriteMessage(results.c_str());
			Assert::IsTrue(counts[Game::Player1Wins] + counts[Game::Player2Wins] + counts[Game::Tie] == 100);
		}
	};
};