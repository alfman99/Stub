#include "pch.h"
#include "Cryptography.hpp"

Cryptography::Cryptography(DecryptKey keyDec) {
	this->data = keyDec;
}

void Cryptography::Decrypt(const BYTE* data, const DWORD size, BYTE* decryptedOut) {
    plusaes::decrypt_cbc(data, size, &this->data.key[0], this->data.key.size(), (const unsigned char(*)[16]) & this->data.iv[0], decryptedOut, size, 0);
}

DecryptKey Cryptography::GetKeyFromResponse(string response) {
	std::vector<BYTE> key(KEY_SIZE);
	std::vector<BYTE> iv(IV_SIZE);
	const char* body_bytes = response.c_str();

	for (int i = 0; i < KEY_SIZE; i++) {
		key[i] = body_bytes[i];
	}

	for (int i = KEY_SIZE; i < KEY_SIZE+IV_SIZE; i++) {
		iv[i-KEY_SIZE] = body_bytes[i];
	}

	DecryptKey returnValue{
		key,
		iv
	};

	return returnValue;
}
