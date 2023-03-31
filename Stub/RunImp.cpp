#include "pch.h"
#include "RunImp.hpp"
#include "DImports.hpp"

RunImp* RunImp::_singleton = nullptr;

RunImp::RunImp() {
    DImports resolver;

    this->dIsDebuggerPresent = resolver._kernel32[OBFUSCATE("IsDebuggerPresent")];
    this->dNtSetInformationThread = resolver._ntdll[OBFUSCATE("NtSetInformationThread")];
}

RunImp* RunImp::GetInstance() {
    if (RunImp::_singleton == nullptr) {
        RunImp::_singleton = new RunImp();
    }
    return RunImp::_singleton;
}
