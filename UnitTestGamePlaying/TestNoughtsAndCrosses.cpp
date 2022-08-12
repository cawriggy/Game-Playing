#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include "../Game-Playing/Noughts_and_Crosses.h"
#include "../Game-Playing/Player_Minimax.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTestGamePlaying
{
	TEST_CLASS(UnitTest_NoughtAndCrosses)
	{
	public:
		using GameClass = Noughts_and_Crosses;

		int GameOutcome(auto actions)
		{
			auto gameObj = GameClass();
			gameObj.Reset();
			for (auto action : actions)
			{
				gameObj.Do(action);
			}
			Logger::WriteMessage(gameObj.GetDisplayString().c_str());
			return gameObj.GetPlayState();
		}


		// ___  Generic Game Tests  ___

		TEST_METHOD(TestStartsUnfinished)
		{
			auto game = GameClass();
			Assert::IsTrue(game.GetPlayState() == Game::Unfinished);
		}

		TEST_METHOD(TestStartsWithValidActions)
		{
			auto game = GameClass();
			auto validActions = game.GetValidActions();
			Assert::IsTrue(validActions.size() > 0);
		}

		TEST_METHOD(TestTakeAValidAction)
		{
			auto game = GameClass();
			auto validActions = game.GetValidActions();
			game.Do(validActions[0]); // an assert within 'Do' may cause test to abort
			//if it works the test will pass otherwise the test may abort
		}

		TEST_METHOD(TestPlayer1Starts)
		{
			auto game = GameClass();
			Assert::IsTrue(game.GetActivePlayer() == 1);
		}

		TEST_METHOD(TestActivePlayer2After1Move)
		{
			auto game = GameClass();
			auto validActions = game.GetValidActions();
			game.Do(validActions[0]);
			Assert::IsTrue(game.GetActivePlayer() == 2);
		}



		// ___  Scenario Tests  ___
		// 
		// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8
		TEST_METHOD(TestP1Win)
		{
			auto actions = { 0, 3, 1, 4, 2 };
			Assert::IsTrue(GameOutcome(actions) == Game::Player1Wins);
		}

		TEST_METHOD(TestP2Win)
		{
			auto actions = { 0, 3, 8, 4, 2, 5 };
			Assert::IsTrue(GameOutcome(actions) == Game::Player2Wins);
		}

		TEST_METHOD(TestTie)
		{
			auto actions = { 0, 4, 8, 1, 7, 6, 2, 5, 3 };
			Assert::IsTrue(GameOutcome(actions) == Game::Tie);
		}


	};


}
