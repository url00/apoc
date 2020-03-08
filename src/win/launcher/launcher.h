#pragma once

#include <vector>
#include <string>
#include <memory>
#include <set>
#include <windows.h>
#include <wingdi.h>

#ifdef LAUNCHER_EXPORTS
#define LAUNCHER_API __declspec(dllexport)
#else
#define LAUNCHER_API __declspec(dllimport)
#endif

typedef const LPWSTR *(*GetLauncherMenu_Proc)();
extern "C" LAUNCHER_API const LPWSTR *GetLauncherMenu();

typedef const size_t (*GetLauncherMenu_Length_Proc)();
extern "C" LAUNCHER_API const size_t GetLauncherMenu_Length();

typedef void (*PaintSometing_Proc)(HWND, HDC);
extern "C" LAUNCHER_API void PaintSometing(HWND hWnd, HDC hdc);
