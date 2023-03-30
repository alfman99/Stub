#pragma once
#include "pch.h"

// Patrón singleton
class RunImp {
private:
    static RunImp* _singleton;

public:

    RunImp();
    RunImp(RunImp& otro) = delete;
    void operator=(const RunImp&) = delete;

    // Singleton logic
    static RunImp* GetInstance();


    // Functions
    decltype(IsDebuggerPresent)* dIsDebuggerPresent;
    decltype(NtSetInformationThread)* dNtSetInformationThread;
};