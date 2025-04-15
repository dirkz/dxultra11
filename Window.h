#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>

namespace dxultra11
{

template <class T> struct Window
{
    static Window<T> *GetAppState(HWND hwnd)
    {
        LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        Window<T> *pState = reinterpret_cast<Window<T> *>(ptr);
        return pState;
    }

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Window<T> *pWindow = nullptr;

        if (uMsg == WM_CREATE)
        {
            CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT *>(lParam);
            pWindow = reinterpret_cast<Window<T> *>(pCreate->lpCreateParams);
            pWindow->WindowCreated(hwnd);
            SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        }
        else
        {
            pWindow = GetAppState(hwnd);
        }

        switch (uMsg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE: {
            int width = LOWORD(lParam); // Macro to get the low-order word.
            int height = HIWORD(lParam);
            return 0;
        }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    Window(HINSTANCE hInstance, int nCmdShow)
    {
        std::wstring classname = T::WindowClass();
        std::wstring title = T::WindowTitle();

        WNDCLASS wc{};
        wc.hInstance = hInstance;
        wc.lpszClassName = classname.c_str();
        wc.lpfnWndProc = WindowProc;

        RegisterClass(&wc);

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

        HWND hwnd = CreateWindowEx(0,                   // Optional window styles.
                                   classname.c_str(),   // Window class
                                   title.c_str(),       // Window text
                                   WS_OVERLAPPEDWINDOW, // Window style

                                   // Size and position
                                   x, y, width, height,

                                   NULL,      // Parent window
                                   NULL,      // Menu
                                   hInstance, // Instance handle
                                   this       // Additional application data
        );

        if (hwnd == NULL)
        {
            throw new std::runtime_error{"Could not create window"};
        }

        ShowWindow(hwnd, nCmdShow);
    }

    void WindowCreated(HWND hwnd)
    {
        m_pCallBack = new T(hwnd);
    }

    int RunMessageLoop()
    {
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
    }

  private:
    T *m_pCallBack = nullptr;
};

} // namespace dxultra11
