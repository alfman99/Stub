#pragma once
#include "pch.h"

#define PASS_SIZE 16
#define IV_SIZE 16

class Cryptography {
public:
	static void Decrypt(const BYTE* data, const DWORD size, BYTE* decrypted);
};

