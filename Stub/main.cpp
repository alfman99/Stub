#include "pch.h"

#include "RunImp.hpp"
#include "AntiDebugging.hpp"

#pragma optimize("\Ox", on)


int main() {

    AntiDebugging* antiDbg = new AntiDebugging();
    antiDbg->start();

    cout << "esperando..." << endl;
    cin.get();

    delete antiDbg;

    return 0;
}
