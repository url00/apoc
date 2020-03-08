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

extern "C" LAUNCHER_API const LPWSTR *GetLauncherMenu();
