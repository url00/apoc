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

HMODULE launcher_handle;

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

    launcher_handle = LoadLibraryExW(L"launcher.dll", NULL, NULL);
    if (launcher_handle == NULL)
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

static unsigned int x = 0;
static unsigned int y = 0;

void DrawTestMessage()
{
    RECT rc;
    GetClientRect(windowHandle, &rc);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    TCHAR message[256];
    // USES_CONVERSION;
    // _tcscpy(message, A2T((LPSTR)GetLauncherMenu()[0]));

    x = x + 1;
    x = x % rc.right;
    y = y + 1;
    y = y % rc.bottom;
    ExtTextOut(
        hdc,
        x,
        y,
        0,
        &rc,
        message,
        lstrlen(message),
        NULL);
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
        DrawTestMessage();

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
