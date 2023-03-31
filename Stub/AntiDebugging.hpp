#pragma once
#include "pch.h"
#include "RunImp.hpp"

class AntiDebugging {
protected:
	atomic<bool> isRunning;
	thread* tTask;

	void LoopCheckDebugger(atomic<bool>& stop, unsigned int sleep);
	bool HideThread(HANDLE handle);
	void KillIfDebuggerPresent();
    
public:
	AntiDebugging();
	~AntiDebugging();

	void start();
	void stop();
};