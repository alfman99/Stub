#pragma once
#include "pch.h"

#define KEY_SIZE 16
#define IV_SIZE 16

struct DecryptKey {
	vector<BYTE> key;
	vector<BYTE> iv;
};

class Cryptography {
private:
	DecryptKey data;

public:
	Cryptography(DecryptKey keyDec);

	vector<BYTE>* Decrypt(vector<BYTE>* data);
	static DecryptKey GetKeyFromResponse(string response);
};

