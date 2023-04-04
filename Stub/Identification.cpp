#include "pch.h"
#include "Identification.hpp"

// https://www.nickdu.com/?p=838
// Get CPU id
string Identification::GetCPUId() {
	array<int, 4> cpuInfo;
	__cpuid(cpuInfo.data(), 1);
	ostringstream buffer;
	buffer
		<< uppercase << hex << setfill('0')
		<< setw(8) << cpuInfo.at(3)
		<< setw(8) << cpuInfo.at(0);
	return buffer.str();
}

// Get harddrive serial
string Identification::GetHDSerial() {
	RunImp* dImp = RunImp::GetInstance();
	DWORD serial;
	dImp->dGetVolumeInformationA(NULL, NULL, NULL, &serial, NULL, NULL, NULL, NULL);
	return to_string(serial);
}

string Identification::GetHWID() {
    // Create string with both CPU and HD ID
    string hwid = GetCPUId() + "|" + GetHDSerial();
    string hashed;

    // Hash it
    picosha2::hash256_hex_string(hwid, hashed);

#ifdef _DEBUG
	cout << "HWID: " << hashed << endl;
#endif // _DEBUG

    return hashed;
}
