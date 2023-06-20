#include "pch.h"
#include "Logging.hpp"

#define WHITE_COLOR 0xF

void Logging::printFormat(string message) {
#ifdef _DEBUG
	time_t rawTime;
	struct tm timeInfo;
	char buffer[80];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);

	strftime(buffer, sizeof(buffer), "[%H:%M:%S]", &timeInfo);

	cout << buffer << " " << message << endl;
#endif
}

FILE* Logging::InitConsole() {
#ifdef _DEBUG
	FILE* cmdOut;
	AllocConsole();
	freopen_s(&cmdOut, "CONOUT$", "w", stdout); // only output no input
	LOG("-------------- Debug mode --------------", Logging::White);

	return cmdOut;
#endif // _DEBUG

	return nullptr;
}

void Logging::DestroyConsole(FILE* cmdOut) {
#ifdef _DEBUG
	LOG("-------------- Exit --------------", Logging::White);

	// Clear console
	fclose(cmdOut);
	FreeConsole();
#endif // _DEBUG
}

void Logging::White(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_COLOR);
	Logging::printFormat(message);
#endif
}

void Logging::Green(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	Logging::printFormat(message);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_COLOR);
#endif // _DEBUG
}

void Logging::Red(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	Logging::printFormat(message);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE_COLOR);
#endif // _DEBUG
}
