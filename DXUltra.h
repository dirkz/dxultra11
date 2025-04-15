#pragma once

#include "stdafx.h"

namespace dxultra11
{

struct DXUltra
{
    static LPRECT DesiredWindowRect(LPRECT r);
    static std::wstring WindowClass();
    static std::wstring WindowTitle();
};

} // namespace dxultra11
