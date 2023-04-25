#include "pch.h"
#include "JunkRoutines.hpp"

// 1 = Enable logging of junk routines, 0 = Disable logging of junk routines
#define LOG_JUNK_ROUTINES_DEBUG 1

// Determine order of execution in compile time of junk routines so its harder to understand what is going on
void JunkRoutines::DoJunk(const int number) {
	switch (number) {
		case 1: {
			poly_random_order(
				poly_junk(), poly_random_order(Junk1(), Junk2())
			);
			break;
		}
		case 2: {
			poly_random_order(
				poly_junk(), poly_random_order(Junk3(), Junk4())
			);
			break;
		}
		case 3: {
			poly_random_order(
				poly_junk(), poly_random_order(Junk5(), Junk6())
			);
			break;
		}
		default: {
			DoJunk(3);
		}
	}
}

void JunkRoutines::Junk1() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk1");
#endif
	FILETIME lpFileTime;
	if (!DosDateTimeToFileTime(0x21, 0x13CF, &lpFileTime)) {
		poly_random_chance(poly_random(10), Junk2());
	}
}

void JunkRoutines::Junk2() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk2");
#endif
	if (GetTickCount() % 10 == 420) {
		poly_random_chance(poly_random(10), Junk3());
	}
}

void JunkRoutines::Junk3() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk3");
#endif
	LPSTR lpShortPath = NULL;
	if (GetShortPathNameA(OBFUSCATE("./queck.exe"), lpShortPath, NULL) == 0) {
		poly_random_chance(poly_random(10), Junk4());
	}
}

void JunkRoutines::Junk4() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk4");
#endif
	SYSTEM_POWER_STATUS status;
	if (!GetSystemPowerStatus(&status)) {
		poly_random_chance(poly_random(10), Junk5());
	}
}

void JunkRoutines::Junk5() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk5");
#endif
	UINT_PTR out = GetSystemMetrics(SM_CXSCREEN);
	out += GetSystemMetrics(SM_CYSCREEN);
	if (out <= 130) {
		out *= 2;
		poly_random_chance(poly_random(10), Junk6());
	}
}

void JunkRoutines::Junk6() {
#if LOG_JUNK_ROUTINES_DEBUG == 1
	Logging::mGreen("Junk6");
#endif
	char drivestr[3];
	char buf[MAX_PATH];
	DWORD ret;

	/* Find an unused drive letter */
	drivestr[0] = 'a';
	drivestr[1] = ':';
	drivestr[2] = 0;

	if (QueryDosDeviceA(drivestr, buf, sizeof(buf)) == 0) {
		poly_random_chance(poly_random(10), Junk5());
	}
}
