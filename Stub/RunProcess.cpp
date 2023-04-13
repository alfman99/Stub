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

#define MANUAL_MAP

void RunProcess::RunProcessFromMemory(vector<BYTE>* payload, DWORD OEP) {
#ifdef MANUAL_MAP
    ManualMappingDLL mmdll = ManualMappingDLL(payload->data(), payload->size());
    ULONG_PTR moduleBase = mmdll.load();
#else
    ULONG_PTR moduleBase = (ULONG_PTR)LoadLibraryA("app");
#endif // MANUAL_MAP

    ULONG_PTR ep_va = OEP + moduleBase;
    int(*new_main)(int argc, char** argv, char** envp) = (int(*)(int, char**, char**))ep_va;
    
    this->dllThread = new thread(new_main, 0, nullptr, nullptr);
#ifdef _DEBUG
    cout << "Thread started" << endl;
#endif // _DEBUG
}
