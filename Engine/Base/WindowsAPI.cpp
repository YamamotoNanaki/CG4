#include "WindowsAPI.h"
//#include "Debug.h"
#include "imgui_impl_win32.h"

#pragma comment(lib,"winmm.lib")

using namespace IFE;

WindowsAPI* IFE::WindowsAPI::Instance()
{
	static WindowsAPI inst;
	return &inst;
}

void IFE::WindowsAPI::Finalize()
{
	timeEndPeriod(1);
	WindowsAPI::Instance()->Unregister();
}
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void IFE::WindowsAPI::Initialize(int32_t window_width, int32_t window_height, LPCWSTR windowName)
{
	timeBeginPeriod(1);
	w.cbSize = sizeof(WNDCLASSEX);
	w.lpfnWndProc = (WNDPROC)WindowProc; // ウィンドウプロシージャを設定
	w.lpszClassName = L"DirectX12Game"; // ウィンドウクラス名
	w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&w);
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	winWidth = window_width;
	winHeight = window_height;

	//タイトルバー削除
	//AdjustWindowRect(&wrc, WS_POPUP, false); // 自動でサイズ補正

	//// ウィンドウオブジェクトの生成
	//hwnd = CreateWindow(w.lpszClassName, // クラス名
	//	windowName,			        // タイトルバーの文字
	//	WS_POPUP,        // 標準的なウィンドウスタイル
	//	CW_USEDEFAULT,              // 表示X座標（OSに任せる）
	//	CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
	//	wrc.right - wrc.left,       // ウィンドウ横幅
	//	wrc.bottom - wrc.top,   // ウィンドウ縦幅
	//	nullptr,                // 親ウィンドウハンドル
	//	nullptr,                // メニューハンドル
	//	w.hInstance,            // 呼び出しアプリケーションハンドル
	//	nullptr);               // オプション



	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(w.lpszClassName, // クラス名
		windowName,			        // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
		CW_USEDEFAULT,              // 表示X座標（OSに任せる）
		CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
		wrc.right - wrc.left,       // ウィンドウ横幅
		wrc.bottom - wrc.top,   // ウィンドウ縦幅
		nullptr,                // 親ウィンドウハンドル
		nullptr,                // メニューハンドル
		w.hInstance,            // 呼び出しアプリケーションハンドル
		nullptr);               // オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);
}

void IFE::WindowsAPI::Unregister()
{
	UnregisterClass(w.lpszClassName, w.hInstance);
}

bool WindowsAPI::Message()
{
	// メッセージがある？
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg);  //プロシージャにメッセージを送る
	}

	//Xボタンで終了メッセージが来たらゲームループを抜ける
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	return false;
}

HINSTANCE* IFE::WindowsAPI::GetHInstance()
{
	return &w.hInstance;
}

HWND* IFE::WindowsAPI::GetHWnd()
{
	return &hwnd;
}

LRESULT IFE::WindowsAPI::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	// メッセージで分岐
	switch (msg)
	{
	case WM_DESTROY: //ウィンドーが破壊された
		PostQuitMessage(0); // OSに対して、アプリの終了を伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}
