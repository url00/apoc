﻿#include <Windows.h>
#include <cstdlib>
#include "lib_display.h"
#include "lib_display_win.h"

static struct WinHost* host;
void connect_lib(void* host)
{
	::host = (struct WinHost*)host;
}

void print_debug(void* message)
{
	host->QueueStringForDisplay((WCHAR*)message);
}

static const WCHAR* platform = L"win";
void* get_platform()
{
	return &platform;
}
