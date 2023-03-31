#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"

#pragma optimize("x", on)


int main() {
    AntiDebugging antiDbg;
    antiDbg.start();
    
    cin.get();

    cout << "bye" << endl;

    return 0;
}
