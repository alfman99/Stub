#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"

#ifndef _DEBUG
// If RELEASE
#pragma optimize("x", on)
#endif // !_DEBUG

int main() {
    AntiDebugging antiDbg;
    antiDbg.start();

    AntiDumping::DeletePEHeader();

    cin.get();

    cout << "bye" << endl;

    return 0;
}
