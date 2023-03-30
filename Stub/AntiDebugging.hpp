#pragma once
#include "pch.h"
#include "RunImp.hpp"

namespace AntiDebugging {
	bool HideThread(HANDLE handle);
	void KillIfDebuggerPresent();

    void LoopCheckDebugger(atomic<bool>& stop, unsigned int sleep);
};