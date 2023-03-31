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

// This will be running in a separate thread
void AntiDebugging::LoopCheckDebugger(atomic<bool>& running, unsigned int sleep) {
    // Hide this thread
    this->HideThread(GetCurrentThread());
    do {
        this->KillIfDebuggerPresent();
        this_thread::sleep_for(chrono::seconds(sleep));
    } while (running);
}

// Constructor
AntiDebugging::AntiDebugging() {
    this->isRunning = false;
    this->tTask = nullptr;
}

AntiDebugging::~AntiDebugging() {
    this->stop();
}

void AntiDebugging::start() {
    if (this->isRunning) return;
    this->isRunning = true;
    this->tTask = new thread(&AntiDebugging::LoopCheckDebugger, this, ref(this->isRunning), 1);
}

void AntiDebugging::stop() {
    this->isRunning = false;
    this->tTask->join();
    delete this->tTask;
    this->tTask = nullptr;
}

