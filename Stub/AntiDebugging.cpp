#include "pch.h"
#include "AntiDebugging.hpp"

bool AntiDebugging::HideThread(HANDLE handle) {
    RunImp* dImp = RunImp::GetInstance();
    NTSTATUS Status = dImp->dNtSetInformationThread(handle, (THREADINFOCLASS)0x11, NULL, 0);
    return Status == 0x0;
}

void AntiDebugging::KillIfDebuggerPresent() {
    RunImp* dImp = RunImp::GetInstance();
    if (dImp->dIsDebuggerPresent()) {
        exit(0);
    }
}

void AntiDebugging::LoopCheckDebugger(atomic<bool>& running, unsigned int sleep) {
    do {
        // AntiDebugging::KillIfDebuggerPresent();
        cout << "Checking if debugger" << endl;
        cout << "Going to sleep";
        this_thread::sleep_for(chrono::milliseconds(sleep));
    } while (running);
}
