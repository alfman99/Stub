#include "pch.h"
#include "RunProcess.hpp"

RunProcess::RunProcess() {
    this->dllThread = nullptr;
    this->isRunning = false;
}

RunProcess::~RunProcess() {
    if (this->dllThread != nullptr && this->isRunning) {
		this->dllThread->join();
		delete this->dllThread;
        this->isRunning = false;
	}
}

void RunProcess::RunProcessFromMemory(vector<BYTE>* payload, DWORD OEP) {
    HMODULE handle = LoadLibraryA("app");
    // FARPROC main = GetProcAddress(handle, "main");

    ULONG_PTR ep_va = OEP + (ULONG_PTR)handle;
    // ULONG_PTR ep_va = this->GetDllEP((DWORD)handle) + (ULONG_PTR)handle;

    this->dllThread = new thread((void(*)())ep_va);

    cout << "Thread started" << endl;
}
