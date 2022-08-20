#include "pch.h"
#include "CppUnitTest.h"
#include "../Game-Playing/Noughts_and_Crosses.h"
#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Random.h"
//#include "../Game-Playing/Player_Minimax.h"
#include <map>
#include <vector>
#include <cstdio>
#include <cinttypes>
#include <format>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{
	TEST_CLASS(Test_Contest)
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

	TEST_CLASS(Test_Ops) 
	{
		TEST_METHOD(TestSwap)
		{
			std::vector<int> intV = { 0,1,2,3,4,5 };
			Logger::WriteMessage("testing equality\n");
			Assert::IsTrue(intV == std::vector<int>{ 0, 1, 2, 3, 4, 5 });
			
			Logger::WriteMessage("swapping\n");
			std::swap(intV[1], intV[3]);
			
			Logger::WriteMessage("testing equality\n");
			Assert::IsTrue(intV == std::vector<int>{ 0, 3, 2, 1, 4, 5 });

			//Logger::WriteMessage("Note: a failed Assert terminates a test\n");
			//Assert::IsTrue(false);
			//Logger::WriteMessage("this does not get printed\n");

		}



		TEST_METHOD(TestIntSize)
		{
			Logger::WriteMessage(std::format("size of fast  8: {}\n", sizeof(std::uint_fast8_t)).c_str());
			Logger::WriteMessage(std::format("size of fast 16: {}\n", sizeof(std::uint_fast16_t)).c_str());
			Logger::WriteMessage(std::format("size of fast 32: {}\n", sizeof(std::uint_fast32_t)).c_str());
			Logger::WriteMessage(std::format("size of fast 64: {}\n", sizeof(std::uint_fast64_t)).c_str());

			Logger::WriteMessage("\n");

			Logger::WriteMessage(std::format("size of least   8: {}\n", sizeof(std::uint_least8_t)).c_str());
			Logger::WriteMessage(std::format("size of least  16: {}\n", sizeof(std::uint_least16_t)).c_str());
			Logger::WriteMessage(std::format("size of least  32: {}\n", sizeof(std::uint_least32_t)).c_str());
			Logger::WriteMessage(std::format("size of least  64: {}\n", sizeof(std::uint_least64_t)).c_str());


			Logger::WriteMessage("\n");
			Logger::WriteMessage(std::format("size of max: {}\n", sizeof(std::intmax_t)).c_str());

		}

	};

	TEST_CLASS(Test_TranspositionTable)
	{
		TEST_METHOD(Test_stdMap)
		{
			// using std::map::contains from c++ 20
			// may need to change project's c++ language standard to support it (edit project properties)

			//decalre a int to int map
			std::map<int, int> m;

			//add an entry in place
			m.emplace(2, 7);
			Assert::IsTrue(m[2] == 7);

			//std::map::contains from c++ 20
			Assert::IsTrue(m.contains(2));
			
			//erasing removes an element
			m.erase(2);
			Assert::IsTrue(!m.contains(2));
			
			//using m[2] adds a default if not contained
			Assert::IsTrue(m[2] == 0); 
			Assert::IsTrue(m.contains(2));
			
			m.erase(2);
			Assert::IsTrue(!m.contains(2));

			//increment operations work as usual
			Assert::IsTrue(++m[1] == 1);
			Assert::IsTrue(m[1]++ == 1);
			Assert::IsTrue(m[1] == 2);

		}

		struct tt_entry
		{
			int a;
			int b;
		};

		TEST_METHOD(Test_MapVectorToInt_boundCache)
		{
			using state = std::vector<int>;
			using bound = int;
			using boundCache = std::map<state, bound>;

			boundCache upperBound;
			boundCache lowerBound;

			state Board1 = { 0,1,0,
							 2,1,0,
							 0,2,0 };

			state Board2 = { 1,2,1,
							 0,1,0,
							 0,2,0, };

			state Board3 = { 1,2,1,
							 0,1,0,
							 0,2,0, };

			state Board4 = { 0,2,1,
							 0,1,0,
							 0,2,0, };

			Assert::IsTrue(Board1 == Board1);
			Assert::IsTrue(Board2 == Board3);

			lowerBound[Board1] = 3;
			lowerBound[Board2] = 5;

			Assert::IsTrue(lowerBound[Board1] == 3);
			Assert::IsTrue(lowerBound[Board2] == 5);
			Assert::IsTrue(lowerBound[Board3] == 5);
			Assert::IsTrue(lowerBound[Board3] == 5);

			int testValue = 4;
			if (lowerBound.contains(Board3) && lowerBound[Board3] > testValue)
			{
				//quick lookup instead of slower function
			}
			else
			{
				Assert::IsTrue(false);
			}


			//default is not added here
			Assert::IsTrue(!lowerBound.contains(Board4));
			if (lowerBound.contains(Board4))
			{
				if (lowerBound[Board4] > testValue)
				{
					//take advatage of quick lookup
				}
			}
			Assert::IsTrue(!lowerBound.contains(Board4));


			//default is not added here
			Assert::IsTrue(!lowerBound.contains(Board4));
			if (lowerBound.contains(Board4) && lowerBound[Board4] > 1)
			{
				Assert::IsTrue(false);
			}
			Assert::IsTrue(!lowerBound.contains(Board4));


			//  BEWARE 
			//  default IS added in this case
			//Assert::IsTrue(!lowerBound.contains(Board4));
			//if (!(lowerBound.contains(Board4) && lowerBound[Board4] > 1))
			//{
			//	Assert::IsTrue(false);
			//}
			//Assert::IsTrue(!lowerBound.contains(Board4));

		}

		struct TestStruct
		{
			int a;
			int b;
		};

		struct TestStruct2
		{
			int a;
			int b;
			int sum()
			{
				return a + b;
			}
		};

		struct TestStruct3
		{
			int a;
			int b;
			static int ccc;
		};

		struct TestStruct4
		{
			int a;
			int b;
			static const int ccc = 7;
		};

		struct TestStruct5
		{
			int a;
			int b;
			int ccc = 7;
		};

		struct TestStruct6
		{
			int q;
			int w;
			enum bmsks
			{
				a = 1<<0,
				b = 1<<1,
				c = 1<<2,
			};
		};
		TEST_METHOD(Test_Struct)
		{

			//TestStruct3::ccc = 4;
			

			auto n1 = sizeof(TestStruct);
			auto n2 = sizeof(TestStruct2);
			auto n3 = sizeof(TestStruct3);
			auto n4 = sizeof(TestStruct4);
			auto n5 = sizeof(TestStruct5);
			std::string msg;
			msg += std::to_string(n1);
			msg += "  ";
			msg += std::to_string(n2);
			msg += "  ";
			msg += std::to_string(n3);
			msg += "  ";
			msg += std::to_string(n4);
			msg += "  ";
			msg += std::to_string(n5);
			msg += "  ";
			msg += std::to_string(sizeof(TestStruct6));
			Logger::WriteMessage(msg.c_str());

		}


		TEST_METHOD(Test_unpack)
		{
			std::tuple<int, int> arr[] = { {0,1}, {2,3} };
			for (const auto& [a, b] : arr)
			{
				Logger::WriteMessage(std::format("{} {}\n", a, b).c_str());
			}


		}


	};



	TEST_CLASS(Test_ScoreModule)
	{



		struct test_score_module
		{
			int getScore(int x) { return -x * x; }
		};


		struct search_poloicy
		{
			//auto score = []() {return 30; };
			test_score_module *sm = nullptr;
			int getScore() 
			{ 
				if (sm) { return sm->getScore(4); } 
				return 0;
			}
			int sssss(auto g) {};
		};


		TEST_METHOD(test1)
		{
			auto score = []() {return 30; };

			auto tsm = test_score_module();
			auto sp = search_poloicy();
			sp.sm = &tsm;

			int z = sp.getScore();
			Logger::WriteMessage(std::to_string(z).c_str());


		}


	};

}

