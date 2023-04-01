#pragma once
#include "pch.h"
#include "ThreadLoop.hpp"

class AntiDebugging: public ThreadLoop {
public:

private:
	static const vector<string> blacklistedStrings;

	bool isBlacklistedAppRunning();
	bool HideThread(HANDLE handle);
	
	void KillIfDebuggerPresent();
	void KillIfBlacklistedPresent();
	

	// Define father abstract function
	void loop(atomic<bool>& stop, unsigned int sleep);
};