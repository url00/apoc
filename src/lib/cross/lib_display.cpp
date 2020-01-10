#include <Windows.h>
#include "../include/lib_display.h"

extern "C"
{
	/*
	typedef struct DisplayState
	{
		void (*prime_message)(char*, int);
	} DisplayState;

	DisplayState __state = {};
	*/

	void* get_platform(void* (setter(const char*)))
	{
		return setter("win");
	}
}
