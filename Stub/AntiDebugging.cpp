#include "pch.h"
#include "AntiDebugging.hpp"
#include "JunkRoutines.hpp"

// Private member
const vector<string> AntiDebugging::blacklistedProcess {
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

const vector<string> AntiDebugging::blacklistedWindows {
    // Dumping tool
    OBFUSCATE("scylla"),

    // Debuggers
    // x32dbg; x64dbg; ...
    OBFUSCATE("dbg"),

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

void AntiDebugging::KillIfIntegrityCheckFails() {
    JunkRoutines::DoJunk(rand() % 4 + 1);
    if (!__super::checkIntegrity()) {
        Logging::mRed( "[AntiDebugging::KillIfIntegrityCheckFails()] TRIGGERED" );
#ifndef _DEBUG
        exit(-3);
#endif
    }
}

void AntiDebugging::KillIfDebuggerPresent() {
    RunImp* dImp = RunImp::GetInstance();
    if (dImp->dIsDebuggerPresent()) {
        Logging::mRed( "[AntiDebugging::KillIfDebuggerPresent()] TRIGGERED" );
#ifndef _DEBUG
        exit(-4);
#endif
    }
}

void AntiDebugging::KillIfRemoteDebuggerPresent() {
    RunImp* dImp = RunImp::GetInstance();
    BOOL dPresent;
    if (!dImp->dCheckRemoteDebuggerPresent(dImp->dGetCurrentProcess(), &dPresent) || dPresent) {
        Logging::mRed( "[AntiDebugging::KillIfRemoteDebuggerPresent()] TRIGGERED" );
#ifndef _DEBUG
        exit(-5);
#endif
    }
}

bool AntiDebugging::isBlacklistedProcessRunning() {
    RunImp* dImp = RunImp::GetInstance();

    bool found = false;
    PROCESSENTRY32 ProcessEntry = { 0 };
    ProcessEntry.dwSize = sizeof(PROCESSENTRY32W);

    HANDLE handleSnapshot = dImp->dCreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (dImp->dProcess32First(handleSnapshot, &ProcessEntry)) {
        do {
            for (string element : blacklistedProcess) {
                if (string(ProcessEntry.szExeFile).find(element) != string::npos) {
                    found = true;
#ifdef _DEBUG
                    Logging::mRed("[AntiDebugging::isBlacklistedProcessRunning()] ProcessEntry.szExeFile: " + string(ProcessEntry.szExeFile) + " element: " + element);
#endif // _DEBUG
                    break;
                }
            }
        } while (dImp->dProcess32Next(handleSnapshot, &ProcessEntry));
    }

    dImp->dCloseHandle(handleSnapshot);

#ifdef _DEBUG
    return false;
#else
    return found;
#endif
}

bool AntiDebugging::isBlacklistedWindowRunning() {
    RunImp* dImpExt = RunImp::GetInstance();

    bool found = dImpExt->dEnumWindows([](HWND hwnd, LPARAM lParam) {
        RunImp* dImp = RunImp::GetInstance();
        string title(dImp->dGetWindowTextLengthA(hwnd) + 1, '\0');
        dImp->dGetWindowTextA(hwnd, &title[0], title.size());
        // tolowercase
        transform(title.begin(), title.end(), title.begin(), ::tolower);

        // Return TRUE to continue...
        if (title.size() <= 1) return TRUE;

        vector<string>& paramBlacklistedWindows = *reinterpret_cast<vector<string>*>(lParam);
        for (string blackListed : paramBlacklistedWindows) {
            // Return FALSE to go out of callbacks
            if (title.find(blackListed) != string::npos) {
                Logging::mRed("[AntiDebugging::isBlacklistedWindowRunning()] title Found: " + title + " blackListed String: " + blackListed);
                return FALSE;
            }
        }

        return TRUE;
    }, (LPARAM)&blacklistedWindows);

#ifdef _DEBUG
    return false;
#else
    return !found;
#endif


}


void AntiDebugging::KillIfBlacklistedProcessPresent() {
    if (this->isBlacklistedProcessRunning()) {
        Logging::mRed("[AntiDebugging::KillIfBlacklistedProcessPresent()] TRIGGERED");
#ifndef _DEBUG
        exit(-1);
#endif
    }
}

void AntiDebugging::KillIfBlacklistedWindowsPresent() {
    if (this->isBlacklistedWindowRunning()) {
        Logging::mRed("[AntiDebugging::KillIfBlacklistedWindowsPresent()] TRIGGERED");
#ifndef _DEBUG
        exit(-2);
#endif
    }
}

// This will be running in a separate thread in a loop
void AntiDebugging::procedure() {
#ifdef _DEBUG
    Logging::mLog("[AntiDebugging::procedure()]");
#endif // _DEBUG
    this->KillIfDebuggerPresent();
    this->KillIfBlacklistedProcessPresent();
    this->KillIfBlacklistedWindowsPresent();
    this->KillIfRemoteDebuggerPresent();
}
