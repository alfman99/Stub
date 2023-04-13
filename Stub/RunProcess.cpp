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
    ManualMappingDLL* mmdll = new ManualMappingDLL(payload);
    mmdll->load();
    mmdll->execute(OEP);
#else
    HMODULE handle = LoadLibraryA("app");
    ULONG_PTR ep_va = OEP + (ULONG_PTR)handle;

    int(*new_main)() = (int(*)())ep_va;

    new_main();
#endif


#ifdef _DEBUG
    cout << "Thread started" << endl;
#endif // _DEBUG

}
