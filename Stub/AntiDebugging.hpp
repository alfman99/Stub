#pragma once
#include "pch.h"
#include "ThreadLoop.hpp"

class AntiDebugging: public ThreadLoop {
public:
	static bool HideThread(HANDLE handle);

private:
	static const vector<string> blacklistedProcess;
	static const vector<string> blacklistedWindows;

	static bool isBlacklistedProcessRunning();
	static bool isBlacklistedWindowRunning();
	
	void KillIfDebuggerPresent();
	void KillIfBlacklistedProcessPresent();
	void KillIfBlacklistedWindowsPresent();
	
	// Define father abstract function
	void procedure();
};