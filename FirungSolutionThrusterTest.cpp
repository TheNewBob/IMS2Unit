#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common.h"
#include "TestUtils.h"
#include "FiringSolutionThruster.h"
#include "Calc.h"
#include "FiringSolutionUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IMS2Unit
{		
	TEST_CLASS(FiringSolutionThrusterTest)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(NoTorqueTest)
			TEST_DESCRIPTION(L"Tests thrusters in the six cardinal directions, located on the center of mass, so there's no torque")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(NoTorqueTest)
		{
			Logger::WriteMessage(L"\n\nTest: NoTorqueTest\n");

			vector<FiringSolutionThruster> thrusters;
			//set up the thrusters
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(1, 0, 0), 100));
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(-1, 0, 0), 100));
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(0, 1, 0), 100));
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(0, -1, 0), 100));
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(0, 0, 1), 100));
			thrusters.push_back(FiringSolutionThruster(_V(0, 0, 0), _V(0, 0, -1), 100));

			map<THGROUP_TYPE, double> assignement;

			//test the group assignements

			for (UINT i = 0; i < thrusters.size(); ++i)
			{
				bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 1);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}
			
			bool condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[5], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetBehindTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters behind the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetBehindTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetBehindTest\n");
			//set up a quited at the back, pointing towards the back
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(0, 0, -1), _V(0, 0, 1), _V(0, 1, 0));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetFrontTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters behind the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetFrontTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetFrontTest\n");

			//set up a quintet at the front, pointing towards the front
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(0, 0, 1), _V(0, 0, -1), _V(0, -1, 0));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetLeftTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters to the left the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetLeftTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetLeftTest\n");

			//set up a quintet at the front, pointing towards the front
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(-1, 0, 0), _V(1, 0, 0), _V(0, 1, 0));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetRightTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters to the right the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetRightTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetRightTest\n");

			//set up a quintet at the front, pointing towards the front
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(1, 0, 0), _V(-1, 0, 0), _V(0, 1, 0));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetBelowTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters below the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetBelowTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetBelowTest\n");

			//set up a quintet at the front, pointing towards the front
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(0, -1, 0), _V(0, 1, 0), _V(0, 0, -1));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetAboveTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters above the vessel")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetAboveTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetBelowTest\n");

			//set up a quintet at the front, pointing towards the front
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(0, 1, 0), _V(0, -1, 0), _V(0, 0, 1));

			//the first thruster should only be in one group
			bool condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[0], 1);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			//all other thrusters should be in two groups, one for translation and one for rotation
			for (UINT i = 1; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 2);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetRearLowerRightTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters behind and below to the right, pointing towards the left")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetRearLowerRightTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetRearLowerRightTest\n");
			//set up a quited at the back, pointing towards the back
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(1, -1, -1), _V(-1, 0, 0), _V(0, 1, 0));
			bool condition = false;

			for (UINT i = 0; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 3);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}

		BEGIN_TEST_METHOD_ATTRIBUTE(QuintetFrontUpperLeftTest)
			TEST_DESCRIPTION(L"Puts a quintet of rcs thrusters in front and above to the left, pointing down")
		END_TEST_METHOD_ATTRIBUTE()

			TEST_METHOD(QuintetFrontUpperLeftTest)
		{
			Logger::WriteMessage(L"\n\nTest: QuintetFrontUpperLeftTest\n");
			vector<FiringSolutionThruster> thrusters = FiringSolutionUtils::CreateRcsQuintet(_V(-1, 1, 1), _V(0, -1, 0), _V(0, 0, 1));
			bool condition = false;

			//all thrusters should be in three groups, as their cog offset is the same in all axes
			for (UINT i = 0; i < thrusters.size(); ++i)
			{
				condition = FiringSolutionUtils::IsThrusterInThisManyGroups(thrusters[i], 3);
				Assert::IsTrue(condition, TestUtils::LastMessage());
			}

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[0], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[1], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[2], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[3], THGROUP_ATT_YAWLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(thrusters[4], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}
	};
}