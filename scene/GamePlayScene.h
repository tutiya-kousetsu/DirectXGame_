#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "Input.h"
#include "Enemy.h"
#include "Shoot.h"
#include "GameOver.h"
#include "collision/CollisionPrimitive.h"
/// <summary>
/// ゲームプレイシーン
/// </summary>

class CollisionManager;
class Player;

class GamePlayScene : public BaseScene
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

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

	void Collision();
private:
	
	Sprite* sprite = nullptr;
	Sprite* sprite1 = nullptr;
	Sprite* spriteBG = nullptr;
	DebugCamera* camera = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	Player* player = nullptr;
	Player* objPlayer = nullptr;
	Model* modelPlayer = nullptr;

	Enemy* enemy[4] = {};
	Shoot* shoot = nullptr;
	CollisionManager* collisionManager = nullptr;

	int playerLife = 300;
	int gameScore = 0;
};