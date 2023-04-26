#pragma once
#include "pch.h"
#include <time.h>

#ifdef _DEBUG
	#define LOG(message, func) func(message);
#else
	#define LOG(message, func);
#endif

class Logging {
private:
	static void printFormat(string message);

public:
	static FILE* InitConsole();
	static void DestroyConsole(FILE* cmdOut);

	static void White(string message);
	static void Green(string message);
	static void Red(string message);
};