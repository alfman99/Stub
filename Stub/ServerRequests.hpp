#include "pch.h"

#include <sstream>
#include <iomanip>

class ServerRequests {
public:
	static string GetDecryptKey(const char* projectId, const string HWID);
};