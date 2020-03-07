#include <Windows.h>
#include <Wingdi.h>

HINSTANCE instance;
const unsigned int windowTitle_maxSize = 100;
TCHAR windowTitle[windowTitle_maxSize] = TEXT("apoc");
const unsigned int windowClass_maxSize = 100;
TCHAR windowClass[windowClass_maxSize] = TEXT("apoc");
HWND windowHandle;
HDC hdc = NULL;
HFONT monoFont;
HBRUSH blackBrush;

HMODULE lib_displayHandle;

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
        (LPCWSTR)TEXT("Courier"));

    blackBrush = CreateSolidBrush(RGB(0, 0, 0));

    {
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
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
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(0));
        RegisterClassEx(&wcex);
    }

    HWND MainWindow_hwnd = CreateWindow(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
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

void DrawTestMessage()
{
    RECT rc;
    GetClientRect(windowHandle, &rc);
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    //lib_displayHandle_get_platform(get_platform_impl);
    /*TCHAR message[256];
	ExtTextOut(
		hdc,
		0,
		0,
		0,
		&rc,
		message,
		lstrlen(message),
		NULL
	);*/
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
