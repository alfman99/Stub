#pragma once
#include "pch.h"
#include "DllHelper.hpp"

// https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/

class DImports {
public:
	const DllHelper _kernel32{ OBFUSCATE("kernel32.dll")};
	const DllHelper _ntdll{ OBFUSCATE("ntdll.dll") };
	const DllHelper _user32{ OBFUSCATE("user32.dll") };
};