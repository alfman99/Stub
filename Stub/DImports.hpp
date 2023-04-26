#pragma once
#include "pch.h"
#include "DllHelper.hpp"

// https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/

class DImports {
public:
	const DllHelper _kernel32{ cryptor::create("kernel32.dll").decrypt() };
	const DllHelper _ntdll{ cryptor::create("ntdll.dll").decrypt() };
	const DllHelper _user32{ cryptor::create("user32.dll").decrypt() };
};