#include "pch.h"
#include "ServerRequests.hpp"

string ServerRequests::GetDecryptKey(string HWID) {
	httplib::Client cli(OBFUSCATE("http://localhost:3000"));
	auto res = cli.Get(OBFUSCATE("/api/auth?hwid=") + HWID);
	if (res.error() != httplib::Error::Success) return "";
	if (res->status != 200) return "";

	return res->body;
}
