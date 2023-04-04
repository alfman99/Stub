#include "pch.h"
#include "ServerRequests.hpp"

string ServerRequests::GetDecryptKey(string HWID) {
	httplib::Client cli("http://localhost:3000");
	auto res = cli.Get("/auth");

	if (res.error() != httplib::Error::Success) return "";

	if (res->status != 200) return "";

	return res->body;
}
