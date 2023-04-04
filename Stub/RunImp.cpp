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
    this->dGetModuleHandleA = resolver._kernel32[OBFUSCATE("GetModuleHandleA")];
    this->dVirtualProtect = resolver._kernel32[OBFUSCATE("VirtualProtect")];
    this->dEnumWindows = resolver._user32[OBFUSCATE("EnumWindows")];
    this->dGetWindowTextLengthA = resolver._user32[OBFUSCATE("GetWindowTextLengthA")];
    this->dGetWindowTextA = resolver._user32[OBFUSCATE("GetWindowTextA")];
    this->dCheckRemoteDebuggerPresent = resolver._kernel32[OBFUSCATE("CheckRemoteDebuggerPresent")];
    this->dGetCurrentProcess = resolver._kernel32[OBFUSCATE("GetCurrentProcess")];
    this->dGetVolumeInformationA = resolver._kernel32[OBFUSCATE("GetVolumeInformationA")];
}

RunImp* RunImp::GetInstance() {
    if (RunImp::_singleton == nullptr) {
        RunImp::_singleton = new RunImp();
    }
    return RunImp::_singleton;
}
