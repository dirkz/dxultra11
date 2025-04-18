#include "DXUltra.h"

constexpr DXGI_FORMAT SwapchainFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
constexpr UINT SwapchainBufferCount = 1;

namespace dxultra11
{

void DXUltra::DesiredWindowRect(LPRECT r)
{
    int width = GetSystemMetrics(SM_CXFULLSCREEN);
    int height = GetSystemMetrics(SM_CYFULLSCREEN);

    r->left = width / 4;
    r->right = r->left + width / 2;
    r->top = 0;
    r->bottom = height / 2;
}

std::wstring DXUltra::WindowClass()
{
    return L"DXULTRA11";
}

std::wstring DXUltra::WindowTitle()
{
    return L"DXUltra 11";
}

DXUltra::DXUltra(HWND hwnd, UINT supposedWidth, UINT supposedHeight)
    : m_sampleDescription{1, 0}, m_width{supposedWidth}, m_height{supposedHeight}
{
    HR(CreateDXGIFactory(IID_PPV_ARGS(m_factory.GetAddressOf())));
    HR(m_factory->EnumAdapters(0, m_adapter.GetAddressOf()));

    DXGI_RATIONAL refreshRate{60, 1};
    DXGI_MODE_DESC swapchainMode{supposedWidth,
                                 supposedHeight,
                                 refreshRate,
                                 SwapchainFormat,
                                 DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                                 DXGI_MODE_SCALING_UNSPECIFIED};
    DXGI_SWAP_CHAIN_DESC swapchainDescription{swapchainMode,
                                              m_sampleDescription,
                                              DXGI_USAGE_RENDER_TARGET_OUTPUT,
                                              SwapchainBufferCount,
                                              hwnd,
                                              TRUE,
                                              DXGI_SWAP_EFFECT_DISCARD,
                                              0};

    UINT flags = 0;

#if _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL chosenFeatureLevel = D3D_FEATURE_LEVEL_1_0_CORE;
    HR(D3D11CreateDeviceAndSwapChain(m_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, flags,
                                     &featureLevel, 1, D3D11_SDK_VERSION, &swapchainDescription,
                                     m_swapchain.GetAddressOf(), m_device.GetAddressOf(),
                                     &chosenFeatureLevel, &m_context));

    CreateSwapchainBuffers();
    CreateDepthStencilBufferView();
}

void DXUltra::Resize(HWND hwnd, UINT width, UINT height)
{
    if ((width != m_width || height != m_height) && (width > 0 && height > 0))
    {
        ResizeSwapchain(width, height);
        m_width = width;
        m_height = height;
    }
}

void DXUltra::Draw()
{
    std::array<ID3D11RenderTargetView *, 1> renderTargetViews{m_renderTargetView.Get()};
    m_context->OMSetRenderTargets(0, renderTargetViews.data(), m_depthStencilView.Get());

    D3D11_VIEWPORT viewport{0,   0,  static_cast<FLOAT>(m_width), static_cast<FLOAT>(m_height),
                            0.f, 1.f};
    m_context->RSSetViewports(1, &viewport);

    m_context->ClearDepthStencilView(m_depthStencilView.Get(),
                                     D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

    std::array<FLOAT, 4> color{0.2f, 0.8f, 0.8f, 1.f};
    m_context->ClearRenderTargetView(m_renderTargetView.Get(), color.data());

    HR(m_swapchain->Present(1, 0));
}

BOOL DXUltra::HandleKey(HWND hwnd, WPARAM wParam)
{
    if (wParam == VK_ESCAPE)
    {
        BOOL b = PostMessage(hwnd, WM_CLOSE, 0, 0);
        if (!b)
        {
            CheckLastError();
        }
    }

    return TRUE;
}

float DXUltra::AspectRatio() const
{
    return static_cast<float>(m_width) / m_height;
}

void DXUltra::CreateSwapchainBuffers()
{
    ComPtr<ID3D11Texture2D> backbuffer;
    HR(m_swapchain->GetBuffer(0, IID_PPV_ARGS(backbuffer.ReleaseAndGetAddressOf())));
    m_device->CreateRenderTargetView(backbuffer.Get(), nullptr,
                                     m_renderTargetView.ReleaseAndGetAddressOf());
}

void DXUltra::CreateDepthStencilBufferView()
{
    constexpr UINT mipLevels = 1;
    constexpr UINT arraySize = 1;
    D3D11_TEXTURE2D_DESC desc{m_width,
                              m_height,
                              mipLevels,
                              arraySize,
                              DXGI_FORMAT_D24_UNORM_S8_UINT,
                              m_sampleDescription,
                              D3D11_USAGE_DEFAULT,
                              D3D11_BIND_DEPTH_STENCIL,
                              0,
                              0};

    HR(m_device->CreateTexture2D(&desc, nullptr, m_depthStencilBuffer.ReleaseAndGetAddressOf()));
    HR(m_device->CreateDepthStencilView(m_depthStencilBuffer.Get(), nullptr,
                                        m_depthStencilView.ReleaseAndGetAddressOf()));
}

void DXUltra::ResizeSwapchain(UINT width, UINT height)
{
    m_renderTargetView.Reset();
    HR(m_swapchain->ResizeBuffers(SwapchainBufferCount, width, height, SwapchainFormat, 0));
    CreateSwapchainBuffers();
    CreateDepthStencilBufferView();
}

} // namespace dxultra11