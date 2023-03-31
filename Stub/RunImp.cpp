#include "pch.h"
#include "RunImp.hpp"
#include "DImports.hpp"

RunImp* RunImp::_singleton = nullptr;

RunImp::RunImp() {
    DImports resolver;

    this->dIsDebuggerPresent = resolver._kernel32[OBFUSCATE("IsDebuggerPresent")];
    this->dNtSetInformationThread = resolver._ntdll[OBFUSCATE("NtSetInformationThread")];

    this->dCreateToolhelp32Snapshot = resolver._kernel32[OBFUSCATE("CreateToolhelp32Snapshot")];
    this->dProcess32First = resolver._kernel32[OBFUSCATE("Process32First")];
    this->dProcess32Next = resolver._kernel32[OBFUSCATE("Process32Next")];
    this->dCloseHandle = resolver._kernel32[OBFUSCATE("CloseHandle")];
    this->dGetCurrentThread = resolver._kernel32[OBFUSCATE("GetCurrentThread")];
}

RunImp* RunImp::GetInstance() {
    if (RunImp::_singleton == nullptr) {
        RunImp::_singleton = new RunImp();
    }
    return RunImp::_singleton;
}
