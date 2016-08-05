#include "stdafx.h"
#include "Common.h"
#include "FiringSolutionThruster.h"
#include "Rotations.h"
#include "TestUtils.h"
#include "FiringSolutionUtils.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

FiringSolutionUtils::FiringSolutionUtils()
{
}


FiringSolutionUtils::~FiringSolutionUtils()
{
}


string FiringSolutionUtils::ThGroupToString(THGROUP_TYPE group)
{
	switch (group)
	{
	case THGROUP_ATT_PITCHUP:
		return "PitchUp";
	case THGROUP_ATT_PITCHDOWN:
		return "PitchDown";
	case THGROUP_ATT_YAWRIGHT:
		return "YawRight";
	case THGROUP_ATT_YAWLEFT:
		return "YawLeft";
	case THGROUP_ATT_BANKRIGHT:
		return "BankRight";
	case THGROUP_ATT_BANKLEFT:
		return "BankLeft";
	case THGROUP_ATT_LEFT:
		return "Left";
	case THGROUP_ATT_RIGHT:
		return "Right";
	case THGROUP_ATT_UP:
		return "Up";
	case THGROUP_ATT_DOWN:
		return "Down";
	case THGROUP_ATT_FORWARD:
		return "Forward";
	case THGROUP_ATT_BACK:
		return "Back";
	default:
		return "Unknown";
	}
}


vector<FiringSolutionThruster> FiringSolutionUtils::CreateRcsQuintet(VECTOR3 pos, VECTOR3 dir, VECTOR3 rot)
{
	MATRIX3 matrix = Rotations::GetRotationMatrixFromOrientation(dir, rot);
	VECTOR3 thruster1dir = _V(0, 0, 1);
	VECTOR3 thruster2dir = _V(1, 0, 0);
	VECTOR3 thruster3dir = _V(-1, 0, 0);
	VECTOR3 thruster4dir = _V(0, 1, 0);
	VECTOR3 thruster5dir = _V(0, -1, 0);

	vector<FiringSolutionThruster> thrusters;
	thrusters.push_back(FiringSolutionThruster(pos, mul(matrix, thruster1dir), 100.0));
	thrusters.push_back(FiringSolutionThruster(pos, mul(matrix, thruster2dir), 100.0));
	thrusters.push_back(FiringSolutionThruster(pos, mul(matrix, thruster3dir), 100.0));
	thrusters.push_back(FiringSolutionThruster(pos, mul(matrix, thruster4dir), 100.0));
	thrusters.push_back(FiringSolutionThruster(pos, mul(matrix, thruster5dir), 100.0));

	return thrusters;
}


bool FiringSolutionUtils::IsThrusterInGroup(FiringSolutionThruster thruster, THGROUP_TYPE group)
{
	string testmessage = "\nTesting assignement to group " + FiringSolutionUtils::ThGroupToString(group) + ":\n";
	Logger::WriteMessage(TestUtils::Msg(testmessage));
	LogThruster(thruster);

	auto thgroups = thruster.GetGroups();
	if (thgroups.find(group) != thgroups.end())
	{
		Logger::WriteMessage(L"PASSED!\n");
		return true;
	}

	string error = "Thruster is not in group " + ThGroupToString(group) + "!\n";
	TestUtils::SetMessage(error);
	return false;
}

bool FiringSolutionUtils::IsThrusterInThisManyGroups(FiringSolutionThruster thruster, UINT n)
{
	Logger::WriteMessage(L"\nTesting number of assigned groups:\n");
	LogThruster(thruster);

	UINT ngroups = thruster.GetGroups().size();
	if ( ngroups == n)
	{
		Logger::WriteMessage(L"PASSED!\n"); 
		return true;
	}
	stringstream ss;
	ss << "Thruster has " << ngroups << " groups, should be " << n << "!\n";
	TestUtils::SetMessage(ss.str());
	return false;
}

void FiringSolutionUtils::LogThruster(FiringSolutionThruster thruster)
{
	stringstream ss;
	VECTOR3 pos = thruster.GetPos();
	VECTOR3 dir = thruster.GetDir();
	VECTOR3 force = thruster.GetLinearForce();
	VECTOR3 torque = thruster.GetTorque();
	auto groups = thruster.GetGroups();


	ss << "Pos: " << pos.x << "," << pos.y << "," << pos.z << " Dir: " << dir.x << "," << dir.y << "," << dir.z;
	ss << " Force: " << force.x << "," << force.y << "," << force.z << " Torque: " << torque.x << "," << torque.y << "," << torque.z << " Groups: ";
	for (auto i = groups.begin(); i != groups.end(); ++i)
	{
		ss << ThGroupToString(i->first) << ":" << i->second << "; ";
	}
	ss << endl;

	string log = ss.str();
	Logger::WriteMessage(TestUtils::Msg(log));
}