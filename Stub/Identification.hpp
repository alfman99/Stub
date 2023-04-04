#pragma once
#include "pch.h"
#include <iomanip>

class Identification {
private:
	static string GetCPUId();
	static string GetHDSerial();
public:
	static string GetHWID();
};