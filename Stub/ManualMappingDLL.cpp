#include "pch.h"
#include "ManualMappingDLL.hpp"

void ManualMappingDLL::mapSections() {
	// Map sections

	const IMAGE_DATA_DIRECTORY ImageDataReloc = this->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];
	const IMAGE_DATA_DIRECTORY ImageDataImport = this->pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	for (int i = 0; i < this->pFileHeader->NumberOfSections; i++) {
		PIMAGE_SECTION_HEADER pCurrentHeader = reinterpret_cast<PIMAGE_SECTION_HEADER>((DWORD_PTR)this->pSectionHeaders + (i * sizeof(IMAGE_SECTION_HEADER)));
		if (ImageDataReloc.VirtualAddress >= pCurrentHeader->VirtualAddress && ImageDataReloc.VirtualAddress < (pCurrentHeader->VirtualAddress + pCurrentHeader->Misc.VirtualSize))
			lpImageRelocHeader = pCurrentHeader;
		if (ImageDataImport.VirtualAddress >= pCurrentHeader->VirtualAddress && ImageDataImport.VirtualAddress < (pCurrentHeader->VirtualAddress + pCurrentHeader->Misc.VirtualSize))
			lpImageImportHeader = pCurrentHeader;
		if (pCurrentHeader->SizeOfRawData) {
			RtlCopyMemory(reinterpret_cast<void*>(this->moduleBaseAddress + pCurrentHeader->VirtualAddress), this->dPayload + pCurrentHeader->PointerToRawData, pCurrentHeader->SizeOfRawData);
		}
	}
}

void ManualMappingDLL::resolveRelocations() {
	// Resolve relocations
	DWORD relocOffset = 0;
	IMAGE_DATA_DIRECTORY imageDataReloc = this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC];

	while (relocOffset < imageDataReloc.Size) {
		PIMAGE_BASE_RELOCATION lpImageBaseRelocation = reinterpret_cast<PIMAGE_BASE_RELOCATION>(this->dPayload + lpImageRelocHeader->PointerToRawData + relocOffset);
		relocOffset += sizeof(IMAGE_BASE_RELOCATION);
		const DWORD NumberOfEntries = (lpImageBaseRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(IMAGE_RELOCATION_ENTRY);
		
		for (DWORD i = 0; i < NumberOfEntries; i++) {
			PIMAGE_RELOCATION_ENTRY lpImageRelocationEntry = reinterpret_cast<PIMAGE_RELOCATION_ENTRY>(this->dPayload + lpImageRelocHeader->PointerToRawData + relocOffset);
			relocOffset += sizeof(IMAGE_RELOCATION_ENTRY);

			if (lpImageRelocationEntry->Type == 0) continue;

			DWORD AddressLocation = this->moduleBaseAddress + lpImageBaseRelocation->VirtualAddress + lpImageRelocationEntry->Offset;
			DWORD PatchedAddress = 0;
			RtlCopyMemory(&PatchedAddress, (LPVOID)AddressLocation, sizeof(DWORD_PTR));
			PatchedAddress += this->deltaAddress;
			RtlCopyMemory((LPVOID)AddressLocation, &PatchedAddress, sizeof(DWORD_PTR));
		}
	}
}

void ManualMappingDLL::resolveImports() {
	// Resolve imports
	if (this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size) {
		PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(this->moduleBaseAddress + this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
		while (pImportDescriptor->Name) {
			HMODULE hModule = LoadLibraryA(reinterpret_cast<LPCSTR>(this->moduleBaseAddress + pImportDescriptor->Name));
			PIMAGE_THUNK_DATA pThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(this->moduleBaseAddress + pImportDescriptor->FirstThunk);
			PIMAGE_THUNK_DATA pOriginalThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(this->moduleBaseAddress + pImportDescriptor->OriginalFirstThunk);
			while (pOriginalThunk->u1.AddressOfData) {
				if (pOriginalThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
					pThunk->u1.Function = reinterpret_cast<DWORD>(GetProcAddress(hModule, reinterpret_cast<LPCSTR>(pOriginalThunk->u1.Ordinal & 0xFFFF)));
				}
				else {
					PIMAGE_IMPORT_BY_NAME pImport = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(this->moduleBaseAddress + pOriginalThunk->u1.AddressOfData);
					pThunk->u1.Function = reinterpret_cast<DWORD>(GetProcAddress(hModule, reinterpret_cast<LPCSTR>(pImport->Name)));
				}
				pThunk++;
				pOriginalThunk++;
			}
			pImportDescriptor++;
		}
	}
}

void ManualMappingDLL::executeTLS() {
	// Execute TLS callbacks
	if (this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size) {
		PIMAGE_TLS_DIRECTORY pTlsDirectory = reinterpret_cast<PIMAGE_TLS_DIRECTORY>(this->moduleBaseAddress + this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress);
		PIMAGE_TLS_CALLBACK* pTlsCallback = reinterpret_cast<PIMAGE_TLS_CALLBACK*>(pTlsDirectory->AddressOfCallBacks);
		while (*pTlsCallback) {
			(*pTlsCallback)(reinterpret_cast<void*>(this->moduleBaseAddress), DLL_PROCESS_ATTACH, nullptr);
			pTlsCallback++;
		}
	}
}

void ManualMappingDLL::executeEntryPoint() {
	// Execute entry point
	if (this->pOptionalHeader->AddressOfEntryPoint) {
		reinterpret_cast<DLL_ENTRY_POINT>(this->moduleBaseAddress + this->pOptionalHeader->AddressOfEntryPoint)(this->dPayload, DLL_PROCESS_ATTACH, nullptr);
	}
}

ManualMappingDLL::ManualMappingDLL(BYTE* dPayload, DWORD size) {
	this->dPayload = dPayload;
	this->dPayloadSize = size;

	// TEMP?
	this->lpImageRelocHeader = nullptr;
	this->lpImageImportHeader = nullptr;

	// Get PE headers
	this->pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(this->dPayload);
	this->pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(this->dPayload + this->pDosHeader->e_lfanew);
	this->pSectionHeaders = reinterpret_cast<PIMAGE_SECTION_HEADER>(this->pNtHeaders + 1);
	this->pOptionalHeader = &this->pNtHeaders->OptionalHeader;
	this->pFileHeader = &this->pNtHeaders->FileHeader;

	// Allocate memory for the DLL
	this->moduleBaseAddress = reinterpret_cast<ULONG_PTR>(VirtualAlloc(nullptr, this->pOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));

	// Delta between the base address of the DLL and the base address of the payload
	this->deltaAddress = this->moduleBaseAddress - this->pOptionalHeader->ImageBase;

	// Set the base address of the DLL
	this->pOptionalHeader->ImageBase = this->moduleBaseAddress;
	
	// Copy headers
	RtlCopyMemory((void*)this->moduleBaseAddress, (void*)dPayload, this->pOptionalHeader->SizeOfHeaders);
}

ULONG_PTR ManualMappingDLL::load() {
	// Map sections
	this->mapSections();

	// Resolve relocations
	this->resolveRelocations();

	// Resolve imports
	this->resolveImports();

	// Execute TLS callbacks
	this->executeTLS();

	// Execute entry point
	this->executeEntryPoint();

	return this->moduleBaseAddress;
}

