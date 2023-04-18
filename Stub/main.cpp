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
    PayloadManager* payloadManager = new PayloadManager();


    antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey(payloadManager->GetProjectId(), Identification::GetHWID());

    // Error handling
    if (response.length() <= 0) {
#ifdef _DEBUG
        cout << "HTTP Response \" \"" << endl;
#endif // _DEBUG
        exit(-6);
    };


    antiDbg.KillIfIntegrityCheckFails();
    vector<BYTE>* dPayload = payloadManager->GetDecryptedPayload(response);

#ifdef _DEBUG
    cout << dPayload->data()[0] << dPayload->data()[1] << dPayload->data()[2] << endl;
#endif // _DEBUG

    antiDbg.KillIfIntegrityCheckFails();
    RunProcess* runProcess = new RunProcess();
    runProcess->RunProcessFromMemory(dPayload, payloadManager->GetOEP());

    thread checkIntegrity = thread([&antiDbg]() {
        while (true) {
#ifdef _DEBUG
            cout << "Integrity check..." << endl;
#else
            antiDbg.KillIfIntegrityCheckFails();
#endif // _DEBUG
            Sleep(1000);
        }
    });


    // Delete own PE header
    AntiDumping::DeletePEHeader();

    // Clear memory
    delete payloadManager;
    delete runProcess;

    // Wait for thread to finish
    checkIntegrity.join();

    return 0;

}
