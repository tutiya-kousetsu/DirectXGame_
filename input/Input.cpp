#include "Input.h"
#include "WinApp.h"

#include <cassert>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

Input* Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void Input::Initialize(WinApp* winApp)
{
	HRESULT result;

	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);

	// キーボードデバイスの生成	
	result = dinput->CreateDevice(GUID_SysKeyboard, &devKeyboard, NULL);

	// マウスデバイスの生成	
	result = dinput->CreateDevice(GUID_SysMouse, &devMouse, NULL);

	result = dinput->CreateDevice(GUID_Joystick, &devJoyStick, NULL);

	// 入力データ形式のセット
	result = devKeyboard->SetDataFormat(&c_dfDIKeyboard); // 標準形式

	// 排他制御レベルのセット
	result = devKeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);

	if (devJoyStick != NULL) {
		// 入力データ形式のセット
		result = devJoyStick->SetDataFormat(&c_dfDIJoystick2); // 標準形式

		// 排他制御レベルのセット
		result = devJoyStick->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	}
	this->winApp = winApp;

}

void Input::Update()
{
	HRESULT result;

	{// キーボード
		result = devKeyboard->Acquire();	// キーボード動作開始

		// 前回のキー入力を保存
		memcpy(keyPre, key, sizeof(key));

		// キーの入力
		result = devKeyboard->GetDeviceState(sizeof(key), key);
	}

	{// マウス
		result = devMouse->Acquire();	// マウス動作開始

		// 前回の入力を保存
		mouseStatePre = mouseState;

		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouseState), &mouseState);
	}

	if (devJoyStick != NULL) {
		{// pad
			result = devJoyStick->Acquire();	// pad動作開始

			// 前回の入力を保存
			joyStatePre = joyState;

			// padの入力
			result = devJoyStick->GetDeviceState(sizeof(joyState), &joyState);
		}
	}
}

bool Input::PushKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 0でなければ押している
	if (key[keyNumber]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerKey(BYTE keyNumber)
{
	// 異常な引数を検出
	assert(0 <= keyNumber && keyNumber <= 256);

	// 前回が0で、今回が0でなければトリガー
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::PushMouseLeft()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[0]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::PushMouseMiddle()
{
	// 0でなければ押している
	if (mouseState.rgbButtons[2]) {
		return true;
	}

	// 押していない
	return false;
}

bool Input::TriggerMouseLeft()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[0] && mouseState.rgbButtons[0]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseRight()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[1] && mouseState.rgbButtons[1]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool Input::TriggerMouseMiddle()
{
	// 前回が0で、今回が0でなければトリガー
	if (!mouseStatePre.rgbButtons[2] && mouseState.rgbButtons[2]) {
		return true;
	}

	// トリガーでない
	return false;
}

Input::MouseMove Input::GetMouseMove()
{
	MouseMove tmp{};
	tmp.lX = mouseState.lX;
	tmp.lY = mouseState.lY;
	tmp.lZ = mouseState.lZ;
	return tmp;
}
