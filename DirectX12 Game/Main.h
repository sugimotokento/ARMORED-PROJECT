#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include <io.h>
#include <functional>
#include <Windows.h>

#include <d3d12.h>
#include <d3d12shader.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include<DirectXTex.h>


using namespace DirectX;
using namespace Microsoft::WRL;

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib") 

static const int  SCREEN_WIDTH(1920);	// ウインドウの幅
static const int  SCREEN_HEIGHT(1080);	// ウインドウの高さ

HWND GetWind();
void GameEnd();
