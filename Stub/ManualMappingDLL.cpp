#include "pch.h"
#include "ManualMappingDLL.hpp"

void ManualMappingDLL::mapSections() {
	// Map sections
	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(this->pNtHeaders);
	for (int i = 0; i < this->pFileHeader->NumberOfSections; i++) {
		if (pSectionHeader->SizeOfRawData) {
			memcpy(reinterpret_cast<void*>(this->moduleBaseAddress + pSectionHeader->VirtualAddress), this->dPayload->data() + pSectionHeader->PointerToRawData, pSectionHeader->SizeOfRawData);
		}
		pSectionHeader++;
	}
}

void ManualMappingDLL::resolveRelocations() {
	// Resolve relocations
	if (this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) {
		PIMAGE_BASE_RELOCATION pBaseRelocation = reinterpret_cast<PIMAGE_BASE_RELOCATION>(this->moduleBaseAddress + this->pOptionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
		while (pBaseRelocation->VirtualAddress) {
			DWORD* pRelocationData = reinterpret_cast<DWORD*>(reinterpret_cast<DWORD>(pBaseRelocation) + sizeof(IMAGE_BASE_RELOCATION));
			int relocationCount = (pBaseRelocation->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(DWORD);
			for (int i = 0; i < relocationCount; i++) {
				DWORD relocation = pRelocationData[i];
				DWORD relocationType = relocation >> 12;
				DWORD relocationOffset = relocation & 0xFFF;
				if (relocationType == IMAGE_REL_BASED_HIGHLOW) {
					*reinterpret_cast<DWORD*>(this->moduleBaseAddress + pBaseRelocation->VirtualAddress + relocationOffset) += this->moduleBaseAddress;
				}
			}
			pBaseRelocation = reinterpret_cast<PIMAGE_BASE_RELOCATION>(reinterpret_cast<DWORD>(pBaseRelocation) + pBaseRelocation->SizeOfBlock);
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
		reinterpret_cast<DLL_ENTRY_POINT>(this->moduleBaseAddress + this->pOptionalHeader->AddressOfEntryPoint)(&this->dPayload->data()[0], DLL_PROCESS_ATTACH, nullptr);
	}
}

ManualMappingDLL::ManualMappingDLL(vector<BYTE>* dPayload) {
	this->dPayload = dPayload;

	// Get PE headers
	this->pNtHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(this->dPayload->data() + reinterpret_cast<PIMAGE_DOS_HEADER>(this->dPayload->data())->e_lfanew);
	this->pOptionalHeader = &this->pNtHeaders->OptionalHeader;
	this->pFileHeader = &this->pNtHeaders->FileHeader;

	// Allocate memory for the DLL
	this->moduleBaseAddress = reinterpret_cast<ULONG_PTR>(VirtualAlloc(nullptr, this->pOptionalHeader->SizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
}

void ManualMappingDLL::load() {
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
}

void ManualMappingDLL::execute(DWORD address) {
	// Execute the function
	
	ULONG_PTR ep_va = address + this->moduleBaseAddress;

	int(*new_main)() = (int(*)())ep_va;

	new_main();
}
