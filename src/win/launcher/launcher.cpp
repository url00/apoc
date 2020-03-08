#include "launcher.h"

static const LPWSTR menu[] = {L"Test 1", L"cool 3"};

const LPWSTR *GetLauncherMenu()
{
    return menu;
}

const size_t GetLauncherMenu_Length()
{
    return 2;
}