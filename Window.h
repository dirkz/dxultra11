#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace dxultra11
{

template <class T> struct WindowState
{
    T *pCallBack = nullptr;
};

template <class T> void OpenWindow(HINSTANCE hInstance)
{
    RECT rect;
    LPRECT pRect = T::DesiredWindowRect(&rect);

    int x = CW_USEDEFAULT;
    int y = CW_USEDEFAULT;
    int width = CW_USEDEFAULT;
    int height = CW_USEDEFAULT;

    if (pRect)
    {
        x = pRect->left;
        y = pRect->top;
        width = pRect->right - pRect->left;
        height = pRect->bottom - pRect->top;
    }

    WindowState<T> *pWindowState = new WindowState<T>();

    std::wstring classname = T::WindowClass();
    std::wstring title = T::WindowTitle();

    HWND hwnd = CreateWindowEx(0,                   // Optional window styles.
                               classname.c_str(),   // Window class
                               title.c_str(),       // Window text
                               WS_OVERLAPPEDWINDOW, // Window style

                               // Size and position
                               x, y, width, height,

                               NULL,        // Parent window
                               NULL,        // Menu
                               hInstance,   // Instance handle
                               pWindowState // Additional application data
    );
};

} // namespace dxultra11
