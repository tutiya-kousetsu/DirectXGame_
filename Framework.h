#pragma once

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Audio.h"
#include "Object3d.h"


/// <summary>
/// フレームワーク
/// </summary>
class Framework
{
public:
	/// <summary>
	/// 実行
	/// </summary>
	void Run();
	
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	virtual bool IsEndRequst() { return endRequst; }

protected:
	bool endRequst = false;
	//ポインタ置き場
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	DebugCamera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;
};

