#include "pch.h"
#include "AntiDebugging.hpp"

bool AntiDebugging::HideThread(HANDLE handle) {
    HMODULE ntdllHandle = GetModuleHandle(OBFUSCATE("ntdll.dll"));
    if (!ntdllHandle) return false;

    NtSetInformationThreadFunc NtSetInformationThread = (NtSetInformationThreadFunc)GetProcAddress(ntdllHandle, "NtSetInformationThread");
    if (!NtSetInformationThread) return false;

    NTSTATUS Status = NtSetInformationThread(handle, ThreadHideFromDebugger, NULL, 0);
    return Status == 0x0;
}

void AntiDebugging::KillIfDebugerPresent() {
    if (IsDebuggerPresent()) {
        exit(0);
    }
}