#pragma once
#include "pch.h"
#include "Cryptography.hpp"

class PayloadManager {
private:
	vector<BYTE>* encryptedData;
	DWORD OEP;

	vector<BYTE>* LoadRawData();
	DWORD LoadOEPData();
public:
	// Constructor
	PayloadManager();

	vector<BYTE>* GetDecryptedPayload(string response);
	DWORD GetOEP();
};

