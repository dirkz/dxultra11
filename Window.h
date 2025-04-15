#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace dxultra11
{

template <class T> struct Window
{
    struct State
    {
        T *pCallBack = nullptr;
    };

    static State *GetAppState(HWND hwnd)
    {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        State *pState = reinterpret_cast<State *>(ptr);
        return pState;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        State *pState = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
            pState = reinterpret_cast<State *>(pCreate->lpCreateParams);
            pState->pCallBack = new T(hwnd);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
        }
        else
        {
            pState = GetAppState(hwnd);
        }

        switch (uMsg)
        {
        }

        return TRUE;
    }

    static void Create(HINSTANCE hInstance)
    {
        std::wstring classname = T::WindowClass();
        std::wstring title = T::WindowTitle();

        WNDCLASS wc{};
        wc.hInstance = hInstance;
        wc.lpszClassName = classname.c_str();
        wc.lpfnWndProc = WindowProc;

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

        State *pWindowState = new State();

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
    }
};

} // namespace dxultra11
