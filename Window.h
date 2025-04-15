#pragma once

namespace dxultra11
{

template <class T> struct WindowState
{
    T *pCallBack = nullptr;
};

template <class T> void OpenWindow()
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
};

} // namespace dxultra11
