#include <stdlib.h>
#include <wchar.h>
#define _WIN32_WINNT 0x5000
#include "screen_handler.h"

//Windows API & C based screen picker 

int main(int argc, char *argv[]) {

    if (argc < 2) {
        const unsigned short s = sizeof(unsigned char) / sizeof(*argv[0]);
        printf("Not enough arguments for: %s\n", (unsigned char*)argv[0]);
        printf(" path missed?");
        return 1;
    }
    
    //get path from param
    size_t pathL = sizeof(unsigned char) / sizeof(argv[1]);
    wchar_t wPath[256];
    size_t c_conv;
    errno_t errResult = mbstowcs_s(&c_conv, wPath, 256, argv[1], _TRUNCATE);
    HANDLE file;

    int iTimeSleep = 1000;
    int iSwidth = GetSystemMetrics(SM_CXSCREEN);
    int iSheight = GetSystemMetrics(SM_CYSCREEN);
    BOOL bFcreated = FALSE;
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, HIDE_WINDOW);

    while (TRUE) {
        HDC hdcScreen = GetDC(NULL);
        if (hdcScreen == NULL) {
            return 2;
        }
        HDC hdcBmp = CreateCompatibleDC(hdcScreen);
        if (hdcBmp == NULL) {
            return 3;
        }

        HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, iSwidth, iSheight);
        if (hBitmap == NULL) {
            return 4;
        }

        HGDIOBJ hbitmap_old = SelectObject(hdcBmp, hBitmap);
        if (hbitmap_old == NULL) {
            return 5;
        }

        if (!BitBlt(hdcBmp, 0, 0, iSwidth, iSheight, hdcScreen, 0, 0, SRCCOPY)) {
            return 6;
        }
          
        while (TRUE) {
            if (SaveBitmap(wPath, hBitmap, &file)) {
                break;
            }
        }
        SelectObject(hdcBmp, hbitmap_old);
        DeleteDC(hdcBmp);
        ReleaseDC(NULL, hdcScreen);
        DeleteObject(hBitmap);
        DeleteObject(hbitmap_old);
        Sleep(iTimeSleep);
        CloseHandle(file);
    }

    return 0;
}