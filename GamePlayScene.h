#pragma once

#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
//#include "FbxObject3d.h"

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene
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
	void Draw();

private:
	
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	DebugCamera* camera = nullptr;
	DirectXCommon* dxCommon = nullptr;

	//FbxModel* fbxmodel1 = nullptr;
	//FbxObject3d* fbxobject1 = nullptr;
};

