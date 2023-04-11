#pragma once
#include "pch.h"

class RunProcess {
private:
	thread* dllThread;
public:
	atomic<bool> isRunning;
	RunProcess();
	~RunProcess();

	void RunProcessFromMemory(vector<BYTE>* payload, DWORD OEP);
};