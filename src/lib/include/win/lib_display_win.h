#pragma once
#include <Windows.h>

struct WinHost {
	void (*QueueStringForDisplay)(LPWSTR s);
};
