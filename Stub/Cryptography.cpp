#include "pch.h"
#include "Cryptography.hpp"

const vector<BYTE> key = plusaes::key_from_string(&"EncryptionKey128"); // 16-char = 128-bit

const BYTE iv[16] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
};

void Cryptography::Decrypt(const BYTE* data, const DWORD size, BYTE* decryptedOut) {
    plusaes::decrypt_cbc(data, size, &key[0], key.size(), &iv, decryptedOut, size, 0);
}
