#include "framework.h"
#include "apoc.h"
#include "Wingdi.h"

#define MAX_LOADSTRING 100

HINSTANCE instance;
WCHAR MainWindow_title[MAX_LOADSTRING];
WCHAR MainWindow_class[MAX_LOADSTRING];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	instance = hInstance;

	LoadStringW(instance, IDS_APP_TITLE, MainWindow_title, MAX_LOADSTRING);
	LoadStringW(instance, IDC_APOC, MainWindow_class, MAX_LOADSTRING);

	{
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = instance;
		wcex.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_APOC));
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = MainWindow_class;
		wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
		RegisterClassExW(&wcex);
	}

	HWND MainWindow_hwnd = CreateWindowW(MainWindow_class, MainWindow_title, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	if (!MainWindow_hwnd)
	{
		return 1;
	}
	ShowWindow(MainWindow_hwnd, nCmdShow);
	UpdateWindow(MainWindow_hwnd);



	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_APOC));

	MSG msg = {};
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		RECT rc;
		GetClientRect(hWnd, &rc);
		HRGN bgRgn = CreateRectRgnIndirect(&rc);
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRgn(hdc, bgRgn, hBrush);

		//HBRUSH hBrush2 = CreateSolidBrush(RGB(255, 255, 255));
		ExtTextOut(
			hdc,
			0,
			0,
			0,
			&rc,
			L"hello",
			5,
			nullptr
		);

		DeleteObject(hBrush);

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
