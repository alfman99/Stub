#pragma once
#include "pch.h"
#include "Cryptography.hpp"

#define PROJECT_ID_SIZE 21

struct Payload {
	char projectId[PROJECT_ID_SIZE];
	DWORD OEP;
};

class PayloadManager {
private:
	vector<BYTE>* rawData;
	vector<BYTE>* LoadRawData();

	Payload* mappedData;
	Payload* LoadAdditionalData();

public:
	// Constructor
	PayloadManager();
	~PayloadManager();

	char* GetProjectId();
	DWORD GetOEP();
	vector<BYTE>* GetDecryptedPayload(string response);
};

