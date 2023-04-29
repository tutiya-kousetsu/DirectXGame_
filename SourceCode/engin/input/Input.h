#pragma once
#include <Windows.h>
#include <wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include "WinApp.h"

using namespace Microsoft::WRL;

class Input
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};

public://メンバー関数
	//namespace省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	static Input* GetInstance();

	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();
	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name="kyeNumber">キー番号( DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);
	/// <summary>
	/// キーのトリガーをチェック
	/// </summary>
	/// <param name="kyeNumber">キー番号( DIK_0 等)</param>
	/// <returns>トリガーか</returns>
	bool TriggerKey(BYTE keyNumber);

	/// <summary>
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft();

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseRight();

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle();

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();

	inline POINT GetMousePos() { return mousePos; };
	inline void SetMousePos(const POINT& pos) {
		mousePos = pos;
		// クライアント座標(ゲームウィンドウの座標)からスクリーン座標(パソコンの画面)に変換
		ClientToScreen(winApp->GetHwnd(), &mousePos);
		// 変換後の座標の位置にマウスカーソルの位置をセット
		SetCursorPos(mousePos.x, mousePos.y);
		mousePos = pos;
	}
private://メンバー変数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devKeyboard;
	ComPtr<IDirectInput8> dinput = nullptr;
	//今回の全キーの状態
	BYTE key[256] = {};
	//前回の全キーの状態
	BYTE keyPre[256] = {};

	POINT mousePos;

	ComPtr<IDirectInputDevice8> devMouse;
	DIMOUSESTATE2 mouseState = {};
	DIMOUSESTATE2 mouseStatePre = {};

	ComPtr<IDirectInputDevice8> devJoyStick;
	DIJOYSTATE2 joyState = {};
	DIJOYSTATE2 joyStatePre = {};
private:
	// WindowsAPI
	WinApp* winApp = nullptr;
};