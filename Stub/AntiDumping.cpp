#include "pch.h"
#include "AntiDumping.hpp"

bool AntiDumping::DeletePEHeader() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE selfHandle = dImp->dGetModuleHandleA(NULL);
    
    // Change address protection to PAGE_READWRITE
    DWORD oldProtect = 0;
    SIZE_T sizeOfHeader = 0x1000;
    dImp->dVirtualProtect((LPVOID)selfHandle, sizeOfHeader, PAGE_READWRITE, &oldProtect);

    // Delete PE Header
    SecureZeroMemory(selfHandle, sizeOfHeader);

    // Restore protection
    VirtualProtect((LPVOID)selfHandle, sizeOfHeader, PAGE_READONLY, &oldProtect);

    return true;
}
