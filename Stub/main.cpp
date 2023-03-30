#include "pch.h"

#include "PayloadManager.hpp"
#include "Cryptography.hpp"
#include "AntiDebugging.hpp"





int main() {

    if (!AntiDebugging::HideThread(GetCurrentThread())) exit(0);

    // PayloadManager* pm = new PayloadManager();
    // if (!pm->init()) exit(0);

    // system("pause");
    // cout << "PROBANDOOO!!" << endl;
    // system("pause");
    cout << "D" << endl;

    return 0;
}
