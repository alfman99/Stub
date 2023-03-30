#pragma once
#include "pch.h"

typedef enum _THREADINFOCLASS {
    ThreadHideFromDebugger = 0x11
} THREADINFOCLASS;

typedef NTSTATUS(__stdcall* NtSetInformationThreadFunc)(
    HANDLE ThreadHandle,
    THREADINFOCLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength
);

namespace AntiDebugging {
	bool HideThread(HANDLE handle);
	void KillIfDebugerPresent();
};