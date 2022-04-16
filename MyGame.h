#pragma once
#include <vector>

#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Audio.h"
#include "Object3d.h"
#include "Model.h"
#include "Sprite.h"

/// <summary>
/// ゲーム固有のクラス
/// </summary>
class MyGame
{

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsEndRequst() { return endRequst; }

private:
	bool endRequst = false;
	//ポインタ置き場
	WinApp* winApp = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Audio* audio = nullptr;
	DebugText* debugText = nullptr;
	DebugCamera* camera = nullptr;
	SpriteCommon* spriteCommon = nullptr;

	std::vector<Sprite*> sprites;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
};

