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

private:
	static string message;
};

