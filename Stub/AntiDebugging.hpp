#pragma once
#include "pch.h"
#include "ThreadLoop.hpp"

class AntiDebugging: public ThreadLoop {
public:

private:
	static const vector<string> blacklistedProcess;
	static const vector<string> blacklistedWindows;

	bool isBlacklistedProcessRunning();
	bool isBlacklistedWindowRunning();
	bool HideThread(HANDLE handle);
	
	void KillIfDebuggerPresent();
	void KillIfBlacklistedProcessPresent();
	void KillIfBlacklistedWindowsPresent();
	

	// Define father abstract function
	void loop(atomic<bool>& stop, unsigned int sleep);
};