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

	bool init();

	pair<BYTE*, DWORD> GetEncryptedPayload();
};

