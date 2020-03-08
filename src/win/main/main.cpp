#include <Windows.h>
#include <Wingdi.h>
#include <atlstr.h>
#include "launcher.h"

HINSTANCE instance;
HWND MainWindow_hwnd;
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
GetLauncherMenu_Length_Proc launcher_GetLauncherMenu_Length;

void Invalidate()
{
    MessageBoxW(MainWindow_hwnd, L"Would be invalidating now...", L"Would be invalidating now...", 0);
}

int LoadLauncher()
{
    if (launcher_handle != NULL)
    {
        FreeLibrary(launcher_handle);
    }

    CopyFileW(L"launcher.dll", L"launcher_.dll", false);
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
    launcher_GetLauncherMenu_Length = (GetLauncherMenu_Length_Proc)GetProcAddress(launcher_handle, "GetLauncherMenu_Length");
    if (launcher_GetLauncherMenu_Length == NULL)
    {
        return 1;
    }

    return 0;
}

void ChangeTracker_OnChange()
{
    LoadLauncher();
    Invalidate();
}

DWORD ChangeTracker_bytes_changed;
FILE_NOTIFY_INFORMATION ChangeTracker_changes[100];
OVERLAPPED ChangeTracker_overlapped = {};
HANDLE ChangeTracker_cwd = NULL;

void ChangeTracker_Reset()
{
    SetTimer(MainWindow_hwnd, 2, 500, NULL);
    WCHAR cwd_path[256];
    GetCurrentDirectoryW(256, cwd_path);
    if (ChangeTracker_cwd != NULL)
    {
        CloseHandle(ChangeTracker_cwd);
    }
    ChangeTracker_cwd = CreateFileW(
        cwd_path,
        GENERIC_READ,
        FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);
    ChangeTracker_overlapped.hEvent = CreateEventW(NULL, TRUE, FALSE, NULL);
    auto didReadError = ReadDirectoryChangesW(
                            ChangeTracker_cwd,
                            &ChangeTracker_changes,
                            sizeof(ChangeTracker_changes),
                            false,
                            FILE_NOTIFY_CHANGE_LAST_WRITE,
                            &ChangeTracker_bytes_changed,
                            &ChangeTracker_overlapped,
                            NULL) == 0;
    if (didReadError)
    {
        auto err = GetLastError();
        int a = 0;
        return;
    }
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

    MainWindow_hwnd = CreateWindowW(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
                                    CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
    if (!MainWindow_hwnd)
    {
        return 1;
    }
    ShowWindow(MainWindow_hwnd, nCmdShow);
    UpdateWindow(MainWindow_hwnd);

    ChangeTracker_Reset();

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

    auto menu_size = launcher_GetLauncherMenu_Length();
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
        PAINTSTRUCT ps;
        hdc = BeginPaint(hWnd, &ps);

        SelectObject(hdc, monoFont);
        ClearBackgroundToBlack();
        DrawMenu();

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }
    case WM_TIMER:
    {
        switch (wParam)
        {
        case 2:
        {
            DWORD bytesTransferred;
            auto result = GetOverlappedResult(ChangeTracker_cwd, &ChangeTracker_overlapped, &bytesTransferred, false);
            if (result)
            {
                KillTimer(MainWindow_hwnd, 2);
                ResetEvent(ChangeTracker_overlapped.hEvent);
                ChangeTracker_OnChange();
                ChangeTracker_Reset();
            }
            else
            {
                auto err = GetLastError();
                if (err != ERROR_IO_INCOMPLETE)
                {
                    int a = 0;
                }
            }

            break;
        }
        default:
        {

            break;
        }
        }
        break;
    }
    default:
    {
        // Causes it to "work".
        // SleepEx(1, TRUE);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    }
    return 0;
}
