#pragma once
#include "pch.h"

using DLL_ENTRY_POINT = BOOL (WINAPI*)(void*, DWORD, void*);

typedef struct IMAGE_RELOCATION_ENTRY {
	WORD Offset : 12;
	WORD Type : 4;
} IMAGE_RELOCATION_ENTRY, * PIMAGE_RELOCATION_ENTRY;

class ManualMappingDLL {
private:
	BYTE* dPayload;
	DWORD dPayloadSize;

	PIMAGE_SECTION_HEADER lpImageRelocHeader;
	PIMAGE_SECTION_HEADER lpImageImportHeader;

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS pNtHeaders;
	PIMAGE_SECTION_HEADER pSectionHeaders;
	PIMAGE_OPTIONAL_HEADER pOptionalHeader;
	PIMAGE_FILE_HEADER pFileHeader;
	ULONG_PTR moduleBaseAddress;
	DWORD_PTR deltaAddress;

	void mapSections();
	void resolveRelocations();
	void resolveImports();
	void executeTLS();
	void executeEntryPoint();

public:
	ManualMappingDLL(BYTE* dPayload, DWORD size);

	ULONG_PTR load();
};