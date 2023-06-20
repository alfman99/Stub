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

    char windowsDrive = 'A';
    UINT driveType;

    while (windowsDrive <= 'Z') {
        string driveLetter = string(1, windowsDrive) + ":\\";
        driveType = GetDriveTypeA(driveLetter.c_str());

        if (driveType == DRIVE_FIXED) {
            DWORD serialNumber;
            if (dImp->dGetVolumeInformationA(driveLetter.c_str(), NULL, 0, &serialNumber, NULL, NULL, NULL, 0)) {
                string retVal = to_string(serialNumber);
                return retVal;
            }
        }

        windowsDrive++;
    }

    return "";
}

string Identification::GetHWID() {
    // Create string with both CPU and HD ID
    string CPUId = GetCPUId();
	string HDSerial = GetHDSerial();

#ifdef _DEBUG
	LOG("CPU ID: " + CPUId, Logging::Green);
	LOG("HD Serial: " + HDSerial, Logging::Green);
#endif // _DEBUG

	
	string hwid = CPUId + "|" + HDSerial;
    string hashed;

    // Hash it
    picosha2::hash256_hex_string(hwid, hashed);

#ifdef _DEBUG
	LOG("HWID: " + hashed, Logging::Green);
#endif // _DEBUG

    return hashed;
}
