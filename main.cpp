#include "stdafx.h"

#include "DXUltra.h"
#include "Window.h"

using namespace dxultra11;

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR lpCmdLine, _In_ int nCmdShow)
{
    OutputDebugString(L"Hello, world!\n");
    OpenWindow<DXUltra>();
}
