#include "pch.h"
#include "CppUnitTest.h"
#include "../Game-Playing/Noughts_and_Crosses.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Random.h"
//#include "../Game-Playing/Player_Minimax.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{
	TEST_CLASS(UnitTest_Contest)
	{
	public:
		TEST_METHOD(TestPlaysNoughtsAndCrosses)
		{
			auto game = Noughts_and_Crosses();
			auto player1 = Player_Random();
			auto player2 = Player_Random();
			auto contest = Contest();
			contest.PlayGame(game, player1, player2);
		}

		TEST_METHOD(TestPlaySetsCorrectPlayerId)
		{
			auto game = Noughts_and_Crosses();
			auto player1 = Player_Random();
			auto player2 = Player_Random();
			auto contest = Contest();
			contest.PlayGame(game, player1, player2);
			Assert::IsTrue((player1.GetPlayerId() == 1) && (player2.GetPlayerId() == 2));
		}

		TEST_METHOD(TestPlayNGames)
		{
			auto game = Noughts_and_Crosses();
			auto player1 = Player_Random();
			auto player2 = Player_Random();
			auto contest = Contest();

			int n = 100;
			std::map<Game::PlayState, int> outcomes;
			contest.PlayNGames(game, player1, player2, n, outcomes);
			
			int total = 0;
			total += outcomes[Game::Tie];
			total += outcomes[Game::Player1Wins];
			total += outcomes[Game::Player2Wins];

			Assert::IsTrue(total == n);
		}

	};
}
