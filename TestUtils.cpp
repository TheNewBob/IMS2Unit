#include "stdafx.h"
#include "Common.h"
#include "TestUtils.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

string TestUtils::message = "";

TestUtils::TestUtils()
{
}


TestUtils::~TestUtils()
{
}


const wchar_t *TestUtils::Msg(string msg)
{
	wstring ret(msg.begin(), msg.end());
	return ret.c_str();
}


const wchar_t *TestUtils::LastMessage()
{
	string msg = message;
	message = "";
	return Msg(msg);
}

void TestUtils::SetMessage(string msg)
{
	Logger::WriteMessage(Msg(msg));
	message = msg;
}

string TestUtils::V3ToString(VECTOR3 v)
{
	stringstream ss;
	ss << v.x << ", " << v.y << ", " << v.z;
	return ss.str();
}