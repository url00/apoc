#include "launcher.h"

static const LPWSTR menu[] = {L"Test 1", L"Test 2"};

const LPWSTR *GetLauncherMenu()
{
    return menu;
}

const size_t GetLauncherMenu_Count()
{
    return 2;
}