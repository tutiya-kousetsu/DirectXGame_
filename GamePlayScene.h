#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene : public BaseScene
{
public:
	GamePlayScene(SceneManager* sceneManager);
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:
	
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	Object3d* objPost2 = nullptr;
	DebugCamera* camera = nullptr;
	DirectXCommon* dxCommon = nullptr;

	Fbx_Model* fbxmodel1 = nullptr;
	Fbx_Object3d* fbxobject1 = nullptr;
	Fbx_Object3d* fbxobject2 = nullptr;
};

