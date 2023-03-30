#include "pch.h"

#include "resource.h"
#include "PayloadManager.hpp"
#include "Cryptography.hpp"

PayloadManager::PayloadManager() {
    this->rawData = make_pair(nullptr, 0x0);
}

bool PayloadManager::init() {
    try {
        this->LoadRawData();
    }
    catch (exception e) {
        return false;
    }

    return true;
}

pair<BYTE*, DWORD> PayloadManager::LoadRawData() {
    HMODULE m_hInstance = GetModuleHandle(NULL);
    HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN1), "P_BIN1");

    if (!hResource) {
        throw exception::exception("1");
    }

    HGLOBAL hLoadedResource = LoadResource(m_hInstance, hResource);

    if (!hLoadedResource) {
        throw exception::exception("2");
    }

    LPVOID pLockedResource = LockResource(hLoadedResource);

    if (!pLockedResource) {
        throw exception::exception("3");
    }

    DWORD dwResourceSize = SizeofResource(m_hInstance, hResource);

    if (!dwResourceSize) {
        throw exception::exception("4");
    }

    return make_pair((BYTE*)pLockedResource, dwResourceSize);
}

pair<BYTE*, DWORD> PayloadManager::GetEncryptedPayload() {
    return make_pair(nullptr, 0x0);
}