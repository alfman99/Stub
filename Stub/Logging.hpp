#pragma once
#include "pch.h"
#include <time.h>

class Logging {

private:
	static void printFormat(string message);

public:
	static FILE* InitConsole();
	static void DestroyConsole(FILE* cmdOut);

	static void mLog(string message);
	static void mGreen(string message);
	static void mRed(string message);
};