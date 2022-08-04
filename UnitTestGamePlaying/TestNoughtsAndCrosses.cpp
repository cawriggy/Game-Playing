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

		TEST_METHOD(TestStartsUnfinished)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();
			bool bStartsUnfinished = game.GetPlayState() == Game::Unfinished;
			Assert::IsTrue(bStartsUnfinished);
		}

		TEST_METHOD(TestStartsWithValidActions)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			bool bHasActions = validActions.size() > 0;
			Assert::IsTrue(bHasActions);
		}

		TEST_METHOD(TestTakeAValidAction)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			auto action = validActions[0];

			game.Act(action);
		}


		TEST_METHOD(TestPlayer1Starts)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			int firstActivePlayer = game.GetActivePlayer();
			Assert::IsTrue(firstActivePlayer == 1);
		}

		TEST_METHOD(TestActivePlayer2After1Move)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			std::vector<int> validActions;
			game.GetValidActions(validActions);
			game.Act(validActions[0]);

			Assert::IsTrue(game.GetActivePlayer() == 2);
		}


		TEST_METHOD(TestP1Win)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// p1 wins
			//  1 1 1
			//  2 2 _
			//  _ _ _
			for (int action : {0, 3, 1, 4, 2})
			{
				game.Act(action);
			}

			Assert::IsTrue(game.GetPlayState() == Game::Player1Wins);

		}

		TEST_METHOD(TestP2Win)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			// p2 wins
			//  1 _ 1
			//  2 2 2
			//  _ _ 1
			for (int action : {0, 3, 8, 4, 2, 5})
			{
				game.Act(action);
			}

			Assert::IsTrue(game.GetPlayState() == Game::Player2Wins);

		}

		TEST_METHOD(TestTie)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			//  tie
			//  1 2 1
			//  1 2 2
			//  2 1 1
			for (int action : {0, 4, 8, 1, 7, 6, 2, 5, 3})
			{
				game.Act(action);
			}

			Assert::IsTrue(game.GetPlayState() == Game::Tie);

		}


		TEST_METHOD(TestThrow)
		{
			auto game = Noughts_and_Crosses();
			game.Reset();

			// Board layout
			//  0 1 2
			//  3 4 5
			//  6 7 8

			//  tie
			//  1 2 1
			//  1 2 2
			//  2 1 1
			for (int action : {0, 4, 8, 1, 7, 6, 2, 5, 3})
			{
				game.Act(action);
			}

			//Assert::IsTrue(game.GetPlayState() == Game::Tie);
			int e = 11;
			throw(e);

		}

	};


}
