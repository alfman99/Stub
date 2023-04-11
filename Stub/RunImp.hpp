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
    decltype(CreateToolhelp32Snapshot)* dCreateToolhelp32Snapshot;
    decltype(Process32First)* dProcess32First;
    decltype(Process32Next)* dProcess32Next;
    decltype(CloseHandle)* dCloseHandle;
    decltype(GetCurrentThread)* dGetCurrentThread;
    decltype(GetModuleHandleA)* dGetModuleHandleA;
    decltype(VirtualProtect)* dVirtualProtect;
    decltype(EnumWindows)* dEnumWindows;
    decltype(GetWindowTextLengthA)* dGetWindowTextLengthA;
    decltype(GetWindowTextA)* dGetWindowTextA;
    decltype(CheckRemoteDebuggerPresent)* dCheckRemoteDebuggerPresent;
    decltype(GetCurrentProcess)* dGetCurrentProcess;
    decltype(GetVolumeInformationA)* dGetVolumeInformationA;
    decltype(FindResourceA)* dFindResourceA;
    decltype(LoadResource)* dLoadResource;
    decltype(LockResource)* dLockResource;
    decltype(SizeofResource)* dSizeofResource;
};