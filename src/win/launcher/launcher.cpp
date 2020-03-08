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

void PaintSometing(HWND hWnd, HDC hdc)
{
    RECT rect;
    GetClientRect(hWnd, &rect);

    HDC backbuffDC = CreateCompatibleDC(hdc);

    HBITMAP backbuffer = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);

    int savedDC = SaveDC(backbuffDC);
    SelectObject(backbuffDC, backbuffer);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(backbuffDC, &rect, hBrush);
    DeleteObject(hBrush);

    SelectObject(backbuffDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(backbuffDC, RGB(255, 0, 0));
    SelectObject(backbuffDC, GetStockObject(DC_PEN));
    SetDCPenColor(backbuffDC, RGB(0, 0, 0));

    for (int i = 0; i < 10; i++)
    {
        Rectangle(backbuffDC, 40, 80, 500, 500);
    }

    BitBlt(hdc, 0, 0, rect.right, rect.bottom, backbuffDC, 0, 0, SRCCOPY);
    RestoreDC(backbuffDC, savedDC);

    DeleteObject(backbuffer);
    DeleteDC(backbuffDC);
}