#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <map>
#include <array>
#include "../Game-Playing/Game_Mancala.h"
//#include "../Game-Playing/Contest.h"
#include "../Game-Playing/Player_Alphabeta.h"
#include "../Game-Playing/Player_Random.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTestGamePlaying
{
	TEST_CLASS(Test_MCTS)
	{
	public:

		TEST_METHOD(Test_map)
		{
			std::map<int, int> m;
			m[3] = 6;
			m[1] = 2;
			m[4];

			Assert::IsTrue(m.contains(3));
			Assert::IsTrue(m.contains(4));
			Assert::IsTrue(m[3] == 6);
			Assert::IsTrue(m[1] == 2);
			Assert::IsTrue(m[4] == int());
		}


		TEST_METHOD(Test_mapStruct)
		{
			struct MyKey
			{
				int x = 5;
				std::array<int, 6> arr = { 0,1,2,3,4,5 };
				bool operator<(const MyKey& other) const
				{
					if (x < other.x) { return true; }
					return arr < other.arr;
				}
			};

			struct MyVal
			{
				int y = 7;
				bool operator==(const MyVal& other) const
				{
					return y == other.y;
				}
			};


			MyKey k1, k2, k3;
			k1.x = 11;
			k2.x = 22;
			k3.x = 2;

			MyVal v1, v2, v3;
			v1.y = 1;
			v2.y = 2;
			//v3.y = 3;

			std::map<MyKey, MyVal> m;
			m[k1] = v1;
			m[k2] = v2;
			m[k3];
			
			Assert::IsTrue(m.contains(k1));
			Assert::IsTrue(m.contains(k3));
			Assert::IsTrue(m[k1] == v1);
			Assert::IsTrue(m[k2] == v2);
			Assert::IsTrue(m[k3] == MyVal());
		}

	};
};