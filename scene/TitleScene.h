#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "Input.h"
#include "TouchableObject.h"
#include "Camera.h"
#include "FollowingCamera.h"
#include "DebugCamera.h"
#include "FrontEnemy.h"
#include "Player.h"

class TitleScene : public BaseScene
{
public:
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

	void CheckCollision();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	Sprite* titleSp = nullptr;
	Sprite* spaceSp = nullptr;
	Sprite* backSp = nullptr;
	//XMFLOAT2 spacePos;
	Input* input = nullptr;
	std::unique_ptr<TouchableObject> floor;
	//天球
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;
	std::unique_ptr<DebugCamera> camera;
	XMFLOAT3 cameraPos;
	std::unique_ptr<Player> player;
	std::unique_ptr<FrontEnemy> enemy;
	XMFLOAT3 enemyPos;
};

