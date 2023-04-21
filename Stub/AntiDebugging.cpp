#include "pch.h"
#include "AntiDebugging.hpp"

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
    if (!__super::checkIntegrity()) {
        exit(-3);
    }
}

void AntiDebugging::KillIfDebuggerPresent() {
#ifdef _DEBUG
    cout << "AntiDebugging::KillIfDebuggerPresent" << endl;
#else // _DEBUG
    RunImp* dImp = RunImp::GetInstance();
    if (dImp->dIsDebuggerPresent()) {
        exit(-4);
    }
#endif
}

void AntiDebugging::KillIfRemoteDebuggerPresent() {
#ifdef _DEBUG
    cout << "AntiDebugging::KillIfRemoteDebuggerPresent" << endl;
#else // _DEBUG
    RunImp* dImp = RunImp::GetInstance();
    BOOL dPresent;
    if (!dImp->dCheckRemoteDebuggerPresent(dImp->dGetCurrentProcess(), &dPresent) || dPresent) {
        exit(-5);
    }
#endif
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
                    cout << "[AntiDebugging::isBlacklistedProcessRunning()] ProcessEntry.szExeFile: " << ProcessEntry.szExeFile << " element: " << element << endl;
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
            if (title.find(blackListed) != string::npos) return FALSE;
        }

        return TRUE;
    }, (LPARAM)&blacklistedWindows);

    return !found;
}


void AntiDebugging::KillIfBlacklistedProcessPresent() {
#ifdef _DEBUG
    cout << "AntiDebugging::KillIfBlacklistedProcessPresent" << endl;
#endif // _DEBUG
    if (this->isBlacklistedProcessRunning()) {
        exit(-1);
    }
}

void AntiDebugging::KillIfBlacklistedWindowsPresent() {
#ifdef _DEBUG
    cout << "AntiDebugging::KillIfBlacklistedWindowsPresent" << endl;
#endif // _DEBUG
    if (this->isBlacklistedWindowRunning()) {
        exit(-2);
    }
}

// This will be running in a separate thread in a loop
void AntiDebugging::procedure() {
#ifdef _DEBUG
    cout << "AntiDebugging::procedure" << endl;
#endif // _DEBUG
    this->KillIfDebuggerPresent();
    this->KillIfBlacklistedProcessPresent();
    this->KillIfBlacklistedWindowsPresent();
    this->KillIfRemoteDebuggerPresent();
}
