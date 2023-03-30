#include "pch.h"

#include "RunImp.hpp"
#include "AntiDebugging.hpp"


int main() {
    AntiDebugging::KillIfDebuggerPresent();
    AntiDebugging::HideThread(GetCurrentThread());

    cout << ":D";

    return 0;
}
