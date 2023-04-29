#pragma once
#include<windows.h>

class WinApp
{
public://静的メンバ関数
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public://メンバ関数
	static WinApp* GetInstance();

	//初期化
	void Initialize();

	//更新
	void Update();

	//終了
	void Finalize();
private:
	//ウィンドウハンドル
	HWND hwnd = nullptr;
	//ウィンドウクラスの設定
	WNDCLASSEX w{};

public:
	//getter
	HWND GetHwnd() { return hwnd; }
	HINSTANCE GetHInstance() { return w.hInstance; }

public:
	static const int window_width = 1280;  // 横幅
	static const int window_height = 720;  // 縦幅

	bool ProcessMessage();
};

