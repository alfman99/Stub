#include "pch.h"

#include "PayloadManager.hpp"
#include "Cryptography.hpp"
#include "AntiDebugging.hpp"

int main() {
    atomic<bool> running(true);
    thread checkDebuggers(AntiDebugging::LoopCheckDebugger, ref(running), 1000);

    cin.get();

    cout << AntiDebugging::HideThread(checkDebuggers.native_handle());

    cin.get();

    running = false;
    checkDebuggers.join();

    return 0;
}
