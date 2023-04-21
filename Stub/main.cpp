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

    FILE* cmdOUT = Logging::InitConsole();


    // Init Antidebugging
    AntiDebugging antiDbg;
    antiDbg.start();


    antiDbg.KillIfIntegrityCheckFails();
    PayloadManager* payloadManager = new PayloadManager();


    antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey(payloadManager->GetProjectId(), Identification::GetHWID());


    // Error handling
    if (response.length() <= 0) {
        Logging::mRed("[Error] Empty HTTP response from server");
        exit(-6);
    };


    antiDbg.KillIfIntegrityCheckFails();
    vector<BYTE>* dPayload = payloadManager->GetDecryptedPayload(response);


    Logging::mGreen("First 3 bytes of decrypted payload: " + dPayload->data()[0] + dPayload->data()[1] + dPayload->data()[2]);


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
    delete runProcess;
    delete payloadManager;

    // Wait for thread to finish
    checkIntegrity.join();

    // Destroy console
    Logging::DestroyConsole(cmdOUT);

    return 0;

}
