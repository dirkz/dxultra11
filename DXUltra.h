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

    void Resize(HWND hwnd, UINT width, UINT height);
    void Draw();
    BOOL HandleKey(HWND hwnd, WPARAM wParam);

    float AspectRatio() const;

  private:
    void CreateSwapchainBuffers();
    void CreateDepthStencilBufferView();
    void ResizeSwapchain(UINT width, UINT height);

    DXGI_SAMPLE_DESC m_sampleDescription;
    ComPtr<IDXGIFactory> m_factory;
    ComPtr<IDXGIAdapter> m_adapter;
    ComPtr<ID3D11Device> m_device;
    ComPtr<IDXGISwapChain> m_swapchain;
    ComPtr<ID3D11DeviceContext> m_context;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    ComPtr<ID3D11Texture2D> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView> m_depthStencilView;
    UINT m_width;
    UINT m_height;
};

} // namespace dxultra11
