#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"
#include "Identification.hpp"

#ifndef _DEBUG
// If RELEASE
#pragma optimize("x", on)
#endif // !_DEBUG

int main() {

    // Init Antidebugging
    AntiDebugging antiDbg;
    antiDbg.start();

    // Delete own PE header
    AntiDumping::DeletePEHeader();

    antiDbg.KillIfIntegrityCheckFails();
    cout << Identification::GetHWID() << endl;

    cin.get();

    // cout << "bye" << endl;

    return 0;
}
