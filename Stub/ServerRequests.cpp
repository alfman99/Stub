#include "pch.h"
#include "ServerRequests.hpp"


string base64Encode(const char* value) {
	string encoded = "";
	int val = 0, valb = -6;
	for (unsigned int i = 0; i < strlen(value); i++) {
		unsigned char c = value[i];
		val = (val << 8) + c;
		valb += 8;
		while (valb >= 0) {
			encoded.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
			valb -= 6;
		}
	}
	if (valb > -6) encoded.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
	while (encoded.size() % 4) encoded.push_back('=');
	return encoded;
}

string ServerRequests::GetDecryptKey(const char* projectId, const string HWID) {
	httplib::Client cli(OBFUSCATE("http://localhost:3000"));
	auto res = cli.Get(OBFUSCATE("/api/auth?hwid=") + HWID + OBFUSCATE("&id=") + base64Encode(projectId));
	if (res.error() != httplib::Error::Success) return "";
	if (res->status != 200) return "";

	return res->body;
}
