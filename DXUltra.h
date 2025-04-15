#pragma once

#include "stdafx.h"

namespace dxultra11
{

struct DXUltra
{
    static void DesiredWindowRect(LPRECT r);
    static std::wstring WindowClass();
    static std::wstring WindowTitle();

    DXUltra(HWND hwnd, UINT supposedWidth, UINT supposedHeight);

    BOOL HandleKey(HWND hwnd, WPARAM wParam);

  private:
    void CreateSwapchainBuffers();
    void CreateDepthStencilBufferView();

    DXGI_SAMPLE_DESC m_sampleDescription;
    ComPtr<IDXGIFactory> m_factory;
    ComPtr<IDXGIAdapter> m_adapter;
    ComPtr<ID3D11Device> m_device;
    ComPtr<IDXGISwapChain> m_swapchain;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};

} // namespace dxultra11
