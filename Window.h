#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <memory>
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
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            return 0;
        }

        case WM_ACTIVATE: {
            WORD action = LOWORD(wParam);
            std::wstring msg = L"*** WM_ACTIVATE with action " + std::to_wstring(action) + L"\n";
            OutputDebugString(msg.c_str());
            bool isBeingActivated = action != WA_INACTIVE;
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

        RECT rect{0, 0, 800, 600};
        LPRECT pRect = T::DesiredWindowRect(&rect);

        DWORD windowStyle = WS_OVERLAPPEDWINDOW;

        AdjustWindowRect(&rect, windowStyle, FALSE);

        int x = CW_USEDEFAULT;
        int y = CW_USEDEFAULT;
        m_initialSupposedWindowWidth = rect.right - rect.left;
        m_initialSupposedWindowHeight = rect.bottom - rect.top;

        if (pRect)
        {
            x = pRect->left;
            y = pRect->top;
        }

        HWND hwnd =
            CreateWindowEx(0,                 // Optional window styles
                           classname.c_str(), // Window class
                           title.c_str(),     // Window text
                           windowStyle,

                           // Size and position
                           x, y, m_initialSupposedWindowWidth, m_initialSupposedWindowHeight,

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
        m_pCallBack.reset(new T(hwnd, m_initialSupposedWindowWidth, m_initialSupposedWindowHeight));
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
    UINT m_initialSupposedWindowWidth;
    UINT m_initialSupposedWindowHeight;
    std::unique_ptr<T> m_pCallBack;
};

} // namespace dxultra11
