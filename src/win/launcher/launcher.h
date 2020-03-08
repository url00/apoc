#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <Windows.h>

#ifdef LAUNCHER_EXPORTS
#define LAUNCHER_API __declspec(dllexport)
#else
#define LAUNCHER_API __declspec(dllimport)
#endif

typedef const LPWSTR *(*GetLauncherMenu_Proc)();
typedef const size_t (*GetLauncherMenu_Length_Proc)();

extern "C" LAUNCHER_API const LPWSTR *GetLauncherMenu();

extern "C" LAUNCHER_API const size_t GetLauncherMenu_Length();