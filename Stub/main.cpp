#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"
#include "ServerRequests.hpp"
#include "PayloadManager.hpp"
#include "Identification.hpp"

#include "RunProcess.hpp"

#ifndef _DEBUG
// If RELEASE
#pragma optimize("x", on)
#endif // !_DEBUG



int main() {
    // Init Antidebugging
    AntiDebugging antiDbg;
    antiDbg.start();

    antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey(Identification::GetHWID());

    // Error handling
    if (response.length() <= 0) {
#ifdef _DEBUG
        cout << "HTTP Response \" \"" << endl;
#endif // _DEBUG
        exit(-6);
    };

    PayloadManager* payloadManager = new PayloadManager();

    antiDbg.KillIfIntegrityCheckFails();
    vector<BYTE>* dPayload = payloadManager->GetDecryptedPayload(response);

    antiDbg.KillIfIntegrityCheckFails();

#ifdef _DEBUG
    cout << dPayload->data()[0] << dPayload->data()[1] << dPayload->data()[2] << endl;
#endif // _DEBUG

    antiDbg.KillIfIntegrityCheckFails();
    RunProcess* runProcess = new RunProcess();
    runProcess->RunProcessFromMemory(nullptr, payloadManager->GetOEP());

    thread checkIntegrity = thread([&antiDbg]() {
        while (true) {
#ifdef _DEBUG
            cout << "Integrity check..." << endl;
#endif // _DEBUG
            antiDbg.KillIfIntegrityCheckFails();
            Sleep(1000);
        }
    });

    // Delete own PE header
    AntiDumping::DeletePEHeader();

    delete payloadManager;
    delete runProcess;

    checkIntegrity.join();

    return 0;

}
