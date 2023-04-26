#include "pch.h"
#include "RunImp.hpp"
#include "DImports.hpp"

RunImp* RunImp::_singleton = nullptr;

RunImp::RunImp() {
    DImports resolver;

    this->dIsDebuggerPresent = resolver._kernel32[cryptor::create("IsDebuggerPresent").decrypt()];
    this->dNtSetInformationThread = resolver._ntdll[cryptor::create("NtSetInformationThread").decrypt()];
    this->dCreateToolhelp32Snapshot = resolver._kernel32[cryptor::create("CreateToolhelp32Snapshot").decrypt()];
    this->dProcess32First = resolver._kernel32[cryptor::create("Process32First").decrypt()];
    this->dProcess32Next = resolver._kernel32[cryptor::create("Process32Next").decrypt()];
    this->dCloseHandle = resolver._kernel32[cryptor::create("CloseHandle").decrypt()];
    this->dGetCurrentThread = resolver._kernel32[cryptor::create("GetCurrentThread").decrypt()];
    this->dGetModuleHandleA = resolver._kernel32[cryptor::create("GetModuleHandleA").decrypt()];
    this->dVirtualProtect = resolver._kernel32[cryptor::create("VirtualProtect").decrypt()];
    this->dEnumWindows = resolver._user32[cryptor::create("EnumWindows").decrypt()];
    this->dGetWindowTextLengthA = resolver._user32[cryptor::create("GetWindowTextLengthA").decrypt()];
    this->dGetWindowTextA = resolver._user32[cryptor::create("GetWindowTextA").decrypt()];
    this->dCheckRemoteDebuggerPresent = resolver._kernel32[cryptor::create("CheckRemoteDebuggerPresent").decrypt()];
    this->dGetCurrentProcess = resolver._kernel32[cryptor::create("GetCurrentProcess").decrypt()];
    this->dGetVolumeInformationA = resolver._kernel32[cryptor::create("GetVolumeInformationA").decrypt()];
    this->dFindResourceA = resolver._kernel32[cryptor::create("FindResourceA").decrypt()];
    this->dLoadResource = resolver._kernel32[cryptor::create("LoadResource").decrypt()];
    this->dLockResource = resolver._kernel32[cryptor::create("LockResource").decrypt()];
    this->dSizeofResource = resolver._kernel32[cryptor::create("SizeofResource").decrypt()];
    this->dGetDriveTypeA = resolver._kernel32[cryptor::create("GetDriveTypeA").decrypt()];
}

RunImp* RunImp::GetInstance() {
    if (RunImp::_singleton == nullptr) {
        RunImp::_singleton = new RunImp();
    }
    return RunImp::_singleton;
}
