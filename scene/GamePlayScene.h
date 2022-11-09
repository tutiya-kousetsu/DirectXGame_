#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "Input.h"
#include "Player.h"
#include "Enemy.h"
#include "PlayerBullet.h"
#include "GameOver.h"
#include "GameClear.h"
#include "Floor/Floor.h"
#include <memory>
/// <summary>
/// ゲームプレイシーン
/// </summary>
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

	void CheckAllCollision();
private:
	
	Sprite* sprite = nullptr;
	Sprite* sprite1 = nullptr;
	Sprite* spriteBG = nullptr;
	Camera* camera = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	Object3d* groundObj = nullptr;
	Model* groundModel = nullptr;

	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;

	//std::unique_ptr<PlayerBullet> playerBullet;
	Player* player = nullptr;
	Enemy* enemy[7] = {};
	PlayerBullet* playerBullet = nullptr;
	EnemyBullet* enemyBullet = nullptr;
	Floor* floor = nullptr;

	int playerLife = 300;
	int gameScore = 0;

	bool flag = true;
};