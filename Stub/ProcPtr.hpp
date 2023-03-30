#pragma once
#include "pch.h"

// https://blog.benoitblanchon.fr/getprocaddress-like-a-boss/

class ProcPtr {
public:
	explicit ProcPtr(FARPROC ptr): _ptr(ptr) {}

	template <typename T, typename = std::enable_if_t<std::is_function_v<T>>>
	operator T* () const {
		return reinterpret_cast<T*>(_ptr);
	}

private:
	FARPROC _ptr;
};
