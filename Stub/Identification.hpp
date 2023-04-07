#pragma once
#include "pch.h"
#include <intrin.h>

class Identification {
private:
	static string GetCPUId();
	static string GetHDSerial();
public:
	static string GetHWID();
};