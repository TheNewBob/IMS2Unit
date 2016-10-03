#include "stdafx.h"
#include "CppUnitTest.h"
#include "Common.h"
#include "TestUtils.h"
#include "FiringSolutionThruster.h"
#include "FSThrusterCollection.h"
#include "Calc.h"
#include "FiringSolutionUtils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace IMS2Unit
{
	TEST_CLASS(FSThrusterTest)
	{
	public:

		BEGIN_TEST_METHOD_ATTRIBUTE(ComplexGroupAssignementTest)
			TEST_DESCRIPTION(L"Puts four rcs quintets at each the vessels front and rear, with small opposite offsets in x and y, then checks if all the thrusters are assigned the correct groups")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(ComplexGroupAssignementTest)
		{
			Logger::WriteMessage(L"\n\nTest: ComplexGroupAssignementTest\n");

			vector<FiringSolutionThruster> front_right_bottom = FiringSolutionUtils::CreateRcsQuintet(_V(1, -1, 10), _V(-1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> front_right_top = FiringSolutionUtils::CreateRcsQuintet(_V(1, 1, 10), _V(-1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> front_left_top = FiringSolutionUtils::CreateRcsQuintet(_V(-1, 1, 10), _V(1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> front_left_bottom = FiringSolutionUtils::CreateRcsQuintet(_V(-1, -1, 10), _V(1, 0, 0), _V(0, 1, 0));

			vector<FiringSolutionThruster> rear_right_bottom = FiringSolutionUtils::CreateRcsQuintet(_V(1, -1, -10), _V(-1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> rear_right_top = FiringSolutionUtils::CreateRcsQuintet(_V(1, 1, -10), _V(-1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> rear_left_top = FiringSolutionUtils::CreateRcsQuintet(_V(-1, 1, -10), _V(1, 0, 0), _V(0, 1, 0));
			vector<FiringSolutionThruster> rear_left_bottom = FiringSolutionUtils::CreateRcsQuintet(_V(-1, -1, -10), _V(1, 0, 0), _V(0, 1, 0));

			//get all thrusters together
			vector<FiringSolutionThruster> allthrusters;
			allthrusters.insert(allthrusters.begin(), front_right_bottom.begin(), front_right_bottom.end());
			allthrusters.insert(allthrusters.begin() + 5, front_right_top.begin(), front_right_top.end());
			allthrusters.insert(allthrusters.begin() + 10, front_left_top.begin(), front_left_top.end());
			allthrusters.insert(allthrusters.begin() + 15, front_left_bottom.begin(), front_left_bottom.end());
			allthrusters.insert(allthrusters.begin() + 20, rear_right_bottom.begin(), rear_right_bottom.end());
			allthrusters.insert(allthrusters.begin() + 25, rear_right_top.begin(), rear_right_top.end());
			allthrusters.insert(allthrusters.begin() + 30, rear_left_top.begin(), rear_left_top.end());
			allthrusters.insert(allthrusters.begin() + 35, rear_left_bottom.begin(), rear_left_bottom.end());

			FSThrusterCollection thrusters(allthrusters);
			
			bool condition = false;

			//all thrusters except those oriented forward and back should be in two groups, as the force in the remaining group 
			//should be too inefficient to be considered.
			//forward and back should be considered too inefficient for any torque and should be only in one group
			for (UINT i = 0; i < 8; ++i)
			{
				for (UINT j = 0; j < 5; ++j)
				{
					UINT idx = i * 5 + j;
					int ngroups = 3;
					if (j == 1 || j == 2)
					{
						ngroups = 1;
					}
					condition = FiringSolutionUtils::IsThrusterInThisManyGroups(allthrusters[idx], ngroups);
					Assert::IsTrue(condition, TestUtils::LastMessage());
				}
			}

			//not going to test all assignements on all of them. Since their assignements interact, if half of them are correct, the other half has to be too.
			//front right bottom
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[0], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[0], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[0], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[1], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[2], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[3], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[3], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[3], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[4], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[4], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[4], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());


			//front right top
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[5], THGROUP_ATT_LEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[5], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[5], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[6], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[7], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[8], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[8], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[8], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[9], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[9], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[9], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());


			//rear left top
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[30], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[30], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[30], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[31], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[32], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[33], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[33], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[33], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[34], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[34], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[34], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());


			//rear left bottom
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[35], THGROUP_ATT_RIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[35], THGROUP_ATT_YAWRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[35], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[36], THGROUP_ATT_BACK);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[37], THGROUP_ATT_FORWARD);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[38], THGROUP_ATT_UP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[38], THGROUP_ATT_PITCHUP);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[38], THGROUP_ATT_BANKLEFT);
			Assert::IsTrue(condition, TestUtils::LastMessage());

			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[39], THGROUP_ATT_DOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[39], THGROUP_ATT_PITCHDOWN);
			Assert::IsTrue(condition, TestUtils::LastMessage());
			condition = FiringSolutionUtils::IsThrusterInGroup(allthrusters[39], THGROUP_ATT_BANKRIGHT);
			Assert::IsTrue(condition, TestUtils::LastMessage());
		}


		BEGIN_TEST_METHOD_ATTRIBUTE(ThrusterSortTest)
			TEST_DESCRIPTION(L"Creates a whole bunch of thrusters and checks if they're sorted correctly for their various group scores")
		END_TEST_METHOD_ATTRIBUTE()

		TEST_METHOD(ThrusterSortTest)
		{
			Logger::WriteMessage(L"\n\nTest: ThrusterSortTest\n");

			vector<vector<FiringSolutionThruster>> thrustervecs;
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(1, -3, 10), _V(-1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(1, 1, 15), _V(-1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(-5, 2, 10), _V(1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(-2, -13, 10), _V(1, 0, 0), _V(0, 1, 0)));

			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(4, -1, -10), _V(-1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(1, 5, -16), _V(-1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(-2, 7, -10), _V(1, 0, 0), _V(0, 1, 0)));
			thrustervecs.push_back(FiringSolutionUtils::CreateRcsQuintet(_V(-7, -3, -21), _V(1, 0, 0), _V(0, 1, 0)));

			vector<FiringSolutionThruster> allthrusters;

			for (UINT i = 0; i < thrustervecs.size(); ++i)
			{
				for (UINT j = 0; j < thrustervecs[i].size(); ++j)
				{
					allthrusters.push_back((thrustervecs[i])[j]);
				}
			}

			FSThrusterCollection thrusters(allthrusters);
			string msg;

			//go through all thruster groups
			for (UINT i = 3; i < 15; ++i)
			{
				vector<FiringSolutionThruster*> curthrusters = thrusters.GetThrustersInGroup((THGROUP_TYPE)i);
				msg = "\nTesting group " + FiringSolutionUtils::ThGroupToString((THGROUP_TYPE)i) + "\n";
				Logger::WriteMessage(TestUtils::Msg(msg));

				msg = Helpers::doubleToString(curthrusters[0]->GetScore((THGROUP_TYPE)i)) + ", ";
				Logger::WriteMessage(TestUtils::Msg(msg));

				//test if the scores in the returned list are sorted correctly
				for (UINT j = 1; j < curthrusters.size(); ++j)
				{
					msg = Helpers::doubleToString(curthrusters[j]->GetScore((THGROUP_TYPE)i));
					if ((j + 1) % 4 == 0)
					{
						msg += "\n";
					}
					else
					{
						msg += ", ";
					}
					Logger::WriteMessage(TestUtils::Msg(msg));
					msg = "Not sorted in descending order!";
					Assert::IsTrue(curthrusters[j - 1]->GetScore((THGROUP_TYPE)i) >= curthrusters[j]->GetScore((THGROUP_TYPE)i), TestUtils::Msg(msg));
				}
			}
		}

	};
}