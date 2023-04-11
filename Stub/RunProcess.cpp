#include "pch.h"
#include "RunProcess.hpp"

RunProcess::RunProcess() {
    this->dllThread = nullptr;
}

RunProcess::~RunProcess() {
    if (this->dllThread != nullptr) {
		this->dllThread->join();
		delete this->dllThread;
        this->dllThread = nullptr;
	}
}

void RunProcess::RunProcessFromMemory(vector<BYTE>* payload, DWORD OEP) {
    // TODO: Change for manual mapping of DLL (not using LoadLibraryA)
    HMODULE handle = LoadLibraryA("app");

    ULONG_PTR ep_va = OEP + (ULONG_PTR)handle;
    // ULONG_PTR ep_va = this->GetDllEP((DWORD)handle) + (ULONG_PTR)handle;

    this->dllThread = new thread((void(*)())ep_va);

#ifdef _DEBUG
    cout << "Thread started" << endl;
#endif // _DEBUG

}
