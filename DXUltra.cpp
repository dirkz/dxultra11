#include "DXUltra.h"

namespace dxultra11
{

LPRECT DXUltra::DesiredWindowRect(LPRECT r)
{
    int width = GetSystemMetrics(SM_CXFULLSCREEN);
    int height = GetSystemMetrics(SM_CYFULLSCREEN);

    r->left = width / 4;
    r->right = r->left + width / 2;
    r->top = 0;
    r->bottom = height / 2;

    return r;
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
{
    ThrowIfFailed(CreateDXGIFactory(IID_PPV_ARGS(m_factory.GetAddressOf())));
    ThrowIfFailed(m_factory->EnumAdapters(0, m_adapter.GetAddressOf()));

    DXGI_MODE_DESC swapchainMode{};

    UINT flags = 0;

#if _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL chosenFeatureLevel = D3D_FEATURE_LEVEL_1_0_CORE;
    ThrowIfFailed(D3D11CreateDevice(m_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, flags,
                                    &featureLevel, 1, D3D11_SDK_VERSION, m_device.GetAddressOf(),
                                    &chosenFeatureLevel, &m_context));
}

} // namespace dxultra11