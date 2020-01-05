#include "framework.h"
#include "apoc.h"
#include "Wingdi.h"

#define MAX_LOADSTRING 100

HINSTANCE instance;
WCHAR windowTitle[MAX_LOADSTRING];
WCHAR windowClass[MAX_LOADSTRING];
HWND windowHandle;
HDC hdc = NULL;
HFONT monoFont;
HBRUSH blackBrush;

HMODULE lib_displayHandle;
typedef void* (__cdecl* lib_displayHandle_get_platform_type)(void* (setter(const char*)));
lib_displayHandle_get_platform_type lib_displayHandle_get_platform;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	instance = hInstance;

	lib_displayHandle = LoadLibrary(TEXT("lib_display.dll"));
	if (lib_displayHandle != NULL)
	{
		lib_displayHandle_get_platform = (lib_displayHandle_get_platform_type)GetProcAddress(lib_displayHandle, "get_platform");
		if (lib_displayHandle_get_platform == NULL)
		{
			return 1;
		}
	}

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
		TEXT("Courier"));

	blackBrush = CreateSolidBrush(RGB(0, 0, 0));


	LoadStringW(instance, IDS_APP_TITLE, windowTitle, MAX_LOADSTRING);
	LoadStringW(instance, IDC_APOC, windowClass, MAX_LOADSTRING);

	{
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = instance;
		wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APOC));
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = windowClass;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassExW(&wcex);
	}

	HWND MainWindow_hwnd = CreateWindowW(windowClass, windowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	if (!MainWindow_hwnd)
	{
		return 1;
	}
	ShowWindow(MainWindow_hwnd, nCmdShow);
	UpdateWindow(MainWindow_hwnd);



	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APOC));

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

char test_buffer[256];

void* get_platform_impl(const char* platform)
{
	strncpy(test_buffer, platform, strlen(platform));
	return nullptr;
}

void DrawTestMessage()
{
	RECT rc;
	GetClientRect(windowHandle, &rc);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));
	lib_displayHandle_get_platform(get_platform_impl);
	TCHAR message[256];
	mbstowcs(message, test_buffer, 256);
	ExtTextOut(
		hdc,
		0,
		0,
		0,
		&rc,
		message,
		lstrlen(message),
		NULL
	);
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
