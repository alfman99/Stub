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

void AntiDebugging::KillIfDebuggerPresent() {
    RunImp* dImp = RunImp::GetInstance();
    if (dImp->dIsDebuggerPresent()) {
        exit(0);
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
                    cout << "ProcessEntry.szExeFile: " << ProcessEntry.szExeFile << endl << "element: " << element << endl;
                    break;
                }
            }
        } while (dImp->dProcess32Next(handleSnapshot, &ProcessEntry));
    }

    dImp->dCloseHandle(handleSnapshot);
    return found;
}

bool AntiDebugging::isBlacklistedWindowRunning() {
    bool found = EnumWindows([](HWND hwnd, LPARAM lParam) {
        string title(GetWindowTextLengthA(hwnd) + 1, '\0');
        GetWindowTextA(hwnd, &title[0], title.size());
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
    if (this->isBlacklistedProcessRunning()) {
        exit(0);
    }
}

void AntiDebugging::KillIfBlacklistedWindowsPresent() {
    if (this->isBlacklistedWindowRunning()) {
        exit(0);
    }
}


// This will be running in a separate thread
void AntiDebugging::loop(atomic<bool>& running, unsigned int sleep) {

#ifndef _DEBUG
    RunImp* dImp = RunImp::GetInstance();
    // Hide this thread
    this->HideThread(dImp->dGetCurrentThread()); 
#endif // !_DEBUG


    do {
        cout << "AntiDebugging::loop" << endl;
        this->KillIfDebuggerPresent();
        this->KillIfBlacklistedProcessPresent();
        this->KillIfBlacklistedWindowsPresent();

        this_thread::sleep_for(chrono::seconds(sleep));
    } while (running);

}
