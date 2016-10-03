#pragma once

using namespace std;


class TestUtils
{
public:
	TestUtils();
	~TestUtils();

	static const wchar_t * Msg(string msg);

	/**
	 * \brief use from anywhere to set a message for the next test
	 */
	static void SetMessage(string msg);
	
	/**
	 * \brief use to retrieve the last message for assert statementes
	 */
	static const wchar_t *LastMessage();

	/**
	 * \brief returns a string representing a VECTOR3 in the format x, y, z
	 */
	static string V3ToString(VECTOR3 v);

private:
	static string message;
};

