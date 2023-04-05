#pragma once
#include "pch.h"
#include "Cryptography.hpp"

class PayloadManager {
private:
	vector<BYTE>* encryptedData;
	vector<BYTE>* LoadRawData();
public:
	// Constructor
	PayloadManager();

	vector<BYTE>* GetDecryptedPayload(string response);
};

