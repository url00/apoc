#ifndef APOC_LIB_DISPLAY_H
#define APOC_LIB_DISPLAY_H
#pragma once

#ifdef WIN32
#define EXPORT __declspec( dllexport )
#endif

extern "C"
{
	EXPORT void* get_platform();
	EXPORT void show_debug_info();
	EXPORT void print_debug(void* message);
	EXPORT void connect_lib(void* host);
}

#endif
