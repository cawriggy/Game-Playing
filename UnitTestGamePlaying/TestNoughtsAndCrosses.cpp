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
				gameObj.Act(action);
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

		

		TEST_METHOD(TestThrow)
		{
			//throw does cause test to fail
			//throw(11);
		}

	};


}


