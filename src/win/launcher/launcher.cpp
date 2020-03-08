#include "launcher.h"

static const LPWSTR menu[] = {L"Test 1", L"neat 3", L"Ok how about this?", L"Yep this works"};

const LPWSTR *GetLauncherMenu()
{
    return menu;
}

const size_t GetLauncherMenu_Length()
{
    return 4;
}