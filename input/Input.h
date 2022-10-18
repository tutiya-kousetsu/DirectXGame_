#pragma once
#include<wrl.h>
#define DIRECTINPUT_VERSION     0x0800   // DirectInpu
#include<dinput.h>
#include"WinApp.h"

//入力
class Input final {
public:
	//namespace省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public://メンバ関数
	static Input* GetInstance();
	//初期化
	void Initialize(WinApp* winApp);
	//更新
	void Update();

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "KeyNumber">キー番号(DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool PushKey(BYTE keyNumber);

	/// <summary>
	/// キーの押下をチェック
	/// </summary>
	/// <param name = "KeyNumber">キー番号(DIK_0 等)</param>
	/// <returns>押されているか</returns>
	bool TriggerKey(BYTE keyNumber);

private:
	//コンストラクタ
	Input() = default;
	//デストラクタ
	~Input() = default;
public:
	//コピーコンストラクタを無効にする
	Input(const Input& obj) = delete;

	Input& operator=(const Input& obj) = delete;
private://メンバ変数
	//キーボードのデバイス
	ComPtr<IDirectInputDevice8> devkeyboard;
	//DirectInputのインスタンス
	ComPtr<IDirectInput8> dinput;
	//全キーの状態
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

	//WindowsAPI
	WinApp* winApp = nullptr;
};