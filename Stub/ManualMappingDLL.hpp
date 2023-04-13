#pragma once
#include "pch.h"

using DLL_ENTRY_POINT = BOOL (WINAPI*)(void*, DWORD, void*);

class ManualMappingDLL {
private:
	vector<BYTE>* dPayload;
	PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	ULONG_PTR moduleBaseAddress;

	void mapSections();
	void resolveRelocations();
	void resolveImports();
	void executeTLS();
	void executeEntryPoint();

public:
	ManualMappingDLL(vector<BYTE>* dPayload);

	void load();
	void execute(DWORD address);
};