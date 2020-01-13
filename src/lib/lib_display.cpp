#include "lib_display.h"

#ifdef WIN32
#define EXPORT __declspec( dllexport )
#endif

extern "C"
{
	EXPORT void show_debug_info()
	{
		auto p = get_platform();
		print_debug(p);
	}
}
