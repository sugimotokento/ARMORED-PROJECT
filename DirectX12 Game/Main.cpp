#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

#include "main.h"
#include"Input.h"
#include "renderer.h"
#include"SceneManager.h"
#include"Call.h"
#include"ModelLoader.h"

#ifdef _DEBUG
#include"ImguiRenderer.h"
#endif // _DEBUG


const LPCWSTR CLASS_NAME = L"AppClass";
const LPCWSTR WINDOW_NAME = L"DX12Game";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool g_isGame;
HWND g_window;

HWND GetWind(){
	return g_window;
}
void GameEnd() {
	g_isGame = false;
}
int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow){

	//// WICファイル読込のため、COMを初期化しておく
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr)){}
		// error

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		(LPCWSTR)CLASS_NAME,
		NULL
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);
	
	// ウィンドウの作成
	g_window = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
		NULL,
		NULL,
		hInstance,
		NULL);




	// 初期化
	Renderer renderer;
#ifdef _DEBUG
	ImguiRenderer imguiRenderer;
	ImguiRenderer::GetInstance()->Initiaize();
#endif // _DEBUG
	Call call;
	ModelLoader::Create();
	SceneManager::Create();
	Input::Create();
	XInput::Create();



	

	// ウインドウの表示
	ShowWindow(g_window, nCmdShow);
	UpdateWindow(g_window);

	//フレームカウント初期化
	DWORD dwExecLastTime;
	DWORD dwCurrentTime;
	timeBeginPeriod(1);
	dwExecLastTime = timeGetTime();
	dwCurrentTime = 0;

	g_isGame = true;
	
	MSG msg;
	while (g_isGame){
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if (msg.message == WM_QUIT){
				// PostQuitMessage()が呼ばれたらループ終了
				break;
			}else{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else{
			dwCurrentTime = timeGetTime();

			//60fps
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60)){

				dwExecLastTime = dwCurrentTime;
				//更新処理
				Input::GetInstance()->Update();
				XInput::GetInstance()->Update();
				SceneManager::GetInstance()->Update();
				
				//描画処理
				SceneManager::GetInstance()->Draw();
			}
		}
	}


	timeEndPeriod(1);	

	// ウィンドウクラスの登録を解除
	UnregisterClass((LPCWSTR)CLASS_NAME, wcex.hInstance);

	// 終了処理
#ifdef _DEBUG
	ImguiRenderer::GetInstance()->Finalize();
#endif // _DEBUG

	ModelLoader::GetInstance()->Finalize();
	
	XInput::Destroy();
	Input::Destroy();
	SceneManager::Destroy();
	ModelLoader::Destroy();
	

	return (int)msg.wParam;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//コールバック
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	switch (uMsg){
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

