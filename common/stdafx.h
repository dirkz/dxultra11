#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <comdef.h>
#include <wrl.h>

#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dxgi1_6.h>

#ifdef _DEBUG
#include <dxgidebug.h>
#endif

#include <array>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <map>
#include <memory>
#include <numbers>
#include <span>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "Error.h"

using Microsoft::WRL::ComPtr;
