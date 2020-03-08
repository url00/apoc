#include <Windows.h>
#include <Wingdi.h>
#include <atlstr.h>
#include "launcher.h"

HINSTANCE instance;
const unsigned int windowTitle_maxSize = 100;
WCHAR windowTitle[windowTitle_maxSize] = L"apoc";
const unsigned int windowClass_maxSize = 100;
WCHAR windowClass[windowClass_maxSize] = L"apoc";
HWND windowHandle;
HDC hdc = NULL;
HFONT monoFont;
HBRUSH blackBrush;

int LoadLauncher();
HMODULE launcher_handle = NULL;
GetLauncherMenu_Proc launcher_GetLauncherMenu;
GetLauncherMenu_Count_Proc launcher_GetLauncherMenu_Count;

DWORD cwd_change_bytes_changed;
FILE_NOTIFY_INFORMATION cwd_changes = {};

void CwdHasChanges(
    DWORD dwErrorCode,
    DWORD dwNumberOfBytesTransfered,
    LPOVERLAPPED lpOverlapped)
{
    LoadLauncher();
}

int LoadLauncher()
{
    if (launcher_handle != NULL)
    {
        FreeLibrary(launcher_handle);
    }

    WCHAR cwd_path[256];
    GetCurrentDirectoryW(256, cwd_path);
    CopyFileW(L"launcher.dll", L"launcher_.dll", false);
    auto err = GetLastError();
    launcher_handle = LoadLibraryExW(L"launcher_.dll", NULL, NULL);
    if (launcher_handle == NULL)
    {
        return 1;
    }
    launcher_GetLauncherMenu = (GetLauncherMenu_Proc)GetProcAddress(launcher_handle, "GetLauncherMenu");
    if (launcher_GetLauncherMenu == NULL)
    {
        return 1;
    }
    launcher_GetLauncherMenu_Count = (GetLauncherMenu_Count_Proc)GetProcAddress(launcher_handle, "GetLauncherMenu_Count");
    if (launcher_GetLauncherMenu_Count == NULL)
    {
        return 1;
    }

    return 0;
}

void CheckForDirectoryChanges()
{
    WCHAR cwd_path[256];
    GetCurrentDirectoryW(256, cwd_path);
    auto cwd = CreateFileW(cwd_path, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    ReadDirectoryChangesW(cwd, &cwd_changes, sizeof(cwd_changes), false, FILE_NOTIFY_CHANGE_LAST_WRITE, &cwd_change_bytes_changed, NULL, (LPOVERLAPPED_COMPLETION_ROUTINE)&CwdHasChanges);
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow)
{
    instance = hInstance;
    monoFont = CreateFontW(
        48,
        0,
        0,
        0,
        FW_ULTRABOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DRAFT_QUALITY,
        FF_DONTCARE,
        L"System");

    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

    {
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = instance;
        wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(0));
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;
        wcex.lpszClassName = windowClass;
        wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(0));
        RegisterClassExW(&wcex);
    }

    if (LoadLauncher() != 0)
    {
        return 1;
    }

    HWND MainWindow_hwnd = CreateWindowW(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
                                         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (!MainWindow_hwnd)
    {
        return 1;
    }
    ShowWindow(MainWindow_hwnd, nCmdShow);
    UpdateWindow(MainWindow_hwnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(0));

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

void ClearBackgroundToBlack()
{
    RECT rc;
    GetClientRect(windowHandle, &rc);
    HRGN r = CreateRectRgnIndirect(&rc);
    FillRgn(hdc, r, blackBrush);
}

void DrawMenu()
{
    RECT rc;
    GetClientRect(windowHandle, &rc);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));

    auto menu_size = launcher_GetLauncherMenu_Count();
    for (size_t i = 0; i < menu_size; i++)
    {
        WCHAR message[256];
        wcscpy(message, launcher_GetLauncherMenu()[i]);
        auto message_length = wcslen(message);
        SIZE message_size = {};
        GetTextExtentPoint32W(hdc, message, message_length, &message_size);
        int y = 0 + ((message_size.cy + 5) * i);
        ExtTextOutW(
            hdc,
            0,
            y,
            0,
            &rc,
            message,
            message_length,
            NULL);
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    windowHandle = hWnd;

    switch (message)
    {
    case WM_PAINT:
    {
        CheckForDirectoryChanges();
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, monoFont);
        ClearBackgroundToBlack();
        DrawMenu();

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
