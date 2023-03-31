#include "pch.h"
#include "AntiDebugging.hpp"

#include <TlHelp32.h>

// Private member
const vector<string> AntiDebugging::blacklistedStrings{
    // Debuggers
    // x32dbg; x64dbg; ...
    OBFUSCATE("dbg"),

    // IDA
    OBFUSCATE("ida.exe"),
    OBFUSCATE("ida64.exe"),

    // CheatEngine
    OBFUSCATE("cheatengine"),

    // HTTPS MITM proxy
    OBFUSCATE("mitmproxy"),
    OBFUSCATE("mitmweb"),
    OBFUSCATE("mitmdump"),
};

// Private functions
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

bool AntiDebugging::isBlacklistedAppRunning() {
    bool found = false;
    PROCESSENTRY32 ProcessEntry = { 0 };
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE handleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(handleSnapshot, &ProcessEntry)) {
        do {
            for (string element : blacklistedStrings) {
                if (string(ProcessEntry.szExeFile).find(element) != string::npos) {
                    found = true;
                    cout << "ProcessEntry.szExeFile:" << ProcessEntry.szExeFile << endl << "element:" << element << endl;
                    break;
                }
            }
        } while (Process32Next(handleSnapshot, &ProcessEntry));
    }

    CloseHandle(handleSnapshot);
    return found;
}


void AntiDebugging::KillIfBlacklistedPresent() {
    if (this->isBlacklistedAppRunning()) {
        exit(0);
    }
}



// This will be running in a separate thread
void AntiDebugging::loop(atomic<bool>& running, unsigned int sleep) {
    // Hide this thread
    this->HideThread(GetCurrentThread());
    do {
        cout << ":D" << endl;
        this->KillIfDebuggerPresent();
        this->KillIfBlacklistedPresent();
        this_thread::sleep_for(chrono::seconds(sleep));
    } while (running);
}
