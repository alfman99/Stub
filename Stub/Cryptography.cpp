#include "pch.h"
#include "Cryptography.hpp"

Cryptography::Cryptography(DecryptKey keyDec) {
	this->data = keyDec;
}

vector<BYTE>* Cryptography::Decrypt(vector<BYTE>* encrypted) {
	unsigned long paddedSize = 0;
	vector<BYTE>* decrypted = new vector<BYTE>(encrypted->size());

    plusaes::decrypt_cbc(&encrypted->data()[0], encrypted->size(), &this->data.key[0], this->data.key.size(), (const unsigned char(*)[16]) & this->data.iv[0], decrypted->data(), decrypted->size(), &paddedSize);

	return decrypted;
}

DecryptKey Cryptography::GetKeyFromResponse(string response) {
	vector<BYTE> key(KEY_SIZE);
	vector<BYTE> iv(IV_SIZE);
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
