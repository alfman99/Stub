#include "pch.h"
#include "Logging.hpp"


void Logging::printFormat(string message) {
	time_t rawTime;
	struct tm timeInfo;
	char buffer[80];

	time(&rawTime);
	localtime_s(&timeInfo, &rawTime);

	strftime(buffer, sizeof(buffer), "[%H:%M:%S]", &timeInfo);

	cout << buffer << " " << message << endl;
}

FILE* Logging::InitConsole() {
#ifdef _DEBUG
	FILE* cmdOut;
	AllocConsole();
	freopen_s(&cmdOut, "CONOUT$", "w", stdout); // only output no input
	Logging::mLog("-------------- Debug mode --------------");

	return cmdOut;
#endif // _DEBUG

	return nullptr;
}

void Logging::DestroyConsole(FILE* cmdOut) {
#ifdef _DEBUG
	Logging::mLog("-------------- Exit --------------");

	// Clear console
	fclose(cmdOut);
	FreeConsole();
#endif // _DEBUG
}

void Logging::mLog(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xF);
	Logging::printFormat(message);
#endif
}

void Logging::mGreen(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	Logging::printFormat(message);
#endif // _DEBUG
}

void Logging::mRed(string message) {
#ifdef _DEBUG
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	Logging::printFormat(message);
#endif // _DEBUG
}
