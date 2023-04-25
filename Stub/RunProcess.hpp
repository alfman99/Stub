#pragma once
#include "pch.h"
#include "ManualMappingDLL.hpp"
#include "AntiDebugging.hpp"

class RunProcess {
private:
	thread* dllThread;

public:
	atomic<bool> isRunning;
	RunProcess();
	~RunProcess();

	void RunProcessFromMemory(vector<BYTE>* payload, DWORD OEP);
};