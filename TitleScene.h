#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"

class TitleScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon);

	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon);

private:

	Sprite* sprite = nullptr;
	DebugCamera* camera = nullptr;
};

