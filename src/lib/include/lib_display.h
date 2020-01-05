#ifndef APOC_LIB_DISPLAY_H
#define APOC_LIB_DISPLAY_H
#pragma once

#ifdef WIN32
#define EXPORT __declspec( dllexport )
#endif

extern "C"
{
	void* prime_message(char* message, const int message_length);
	void* update_display();
	EXPORT void* get_platform(void* (setter(const char*)));
}

#endif
