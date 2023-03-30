#pragma once
#include "pch.h"
#include "ProcPtr.hpp"

// https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/

class DllHelper {
public:
	// Constructor
	explicit DllHelper(LPCTSTR filename) : _module(LoadLibrary(filename)) {}
	~DllHelper() { FreeLibrary(_module); }

	// [] Operator
	ProcPtr operator[](LPCSTR proc_name) const {
		return ProcPtr(GetProcAddress(_module, proc_name));
	}

	static HMODULE _parent_module;

private:
	HMODULE _module;
};