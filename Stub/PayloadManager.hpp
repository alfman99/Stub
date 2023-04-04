#pragma once
#include "pch.h"
#include "Cryptography.hpp"

class PayloadManager {
private:
	pair<BYTE*, DWORD> rawData;
	pair<BYTE*, DWORD> LoadRawData();
public:
	// Constructor
	PayloadManager();

	pair<BYTE*, DWORD> GetDecryptedPayload(string response);
};

