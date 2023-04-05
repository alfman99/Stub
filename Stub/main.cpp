#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"
#include "ServerRequests.hpp"
#include "PayloadManager.hpp"
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
    // AntiDumping::DeletePEHeader();

    antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey(Identification::GetHWID());

    // Error handling
    if (response.length() <= 0) {
#ifdef _DEBUG
        cout << "HTTP Response \" \"" << endl;
#endif // _DEBUG
        exit(-6);
    };

    PayloadManager* pm = new PayloadManager();

    antiDbg.KillIfIntegrityCheckFails();
    vector<BYTE>* dPayload = pm->GetDecryptedPayload(response);

    antiDbg.KillIfIntegrityCheckFails();
    cout << dPayload->data()[0] << dPayload->data()[1] << dPayload->data()[2] << endl;

    cin.get();

    cout << "bye" << endl;

    return 0;
}
