#pragma once

#include "stdafx.h"

namespace dxultra11
{

struct DXUltra
{
    static LPRECT DesiredWindowRect(LPRECT r);
    static std::wstring WindowClass();
    static std::wstring WindowTitle();

    DXUltra(HWND hwnd, UINT supposedWidth, UINT supposedHeight);

  private:
    ComPtr<IDXGIFactory> m_factory;
    ComPtr<IDXGIAdapter> m_adapter;
    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_context;
};

} // namespace dxultra11
