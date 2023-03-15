#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "PlayerBullet.h"
#include "GameOver.h"
#include "GameClear.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "Obstacle.h"
#include "Floor/Floor.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "Wall.h"
#include "Door.h"
#include <memory>
#include <list>
#include <sstream>

//前方宣言
class Player;
class Line;
class ParticleManager;
class CollisionManager;
class TouchableObject;
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
	
	/// <summary>
	/// 障害物の発生データの読み込み
	/// </summary>
	void LoadObstaclePopData();

	/// <summary>
	/// 障害物配置のコマンド更新
	/// </summary>
	void UpdataObstaclePopCommand();


	//前敵の当たり判定
	void FrontColl();
	//左敵の当たり判定
	void LeftColl();
	//右敵の当たり判定
	void RightColl();
	//後ろ敵の当たり判定
	void BackColl();

	void CheckAllCollision();
private:
	//スプライト
	Sprite* sprite = nullptr;
	Sprite* LifeSprite = nullptr;
	Sprite* LifeSprite2 = nullptr;
	Sprite* LifeSprite3 = nullptr;
	Sprite* LifeSprite4 = nullptr;
	Sprite* LifeSprite5 = nullptr;
	Sprite* LifeSprite6 = nullptr;
	Sprite* LifeSprite7 = nullptr;
	Sprite* LifeSprite8 = nullptr;
	Sprite* spriteBG = nullptr;
	// カメラ
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	
	//障害物
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;

	//プレイヤー
	Player* player = nullptr;
	PlayerBullet* playerBullet = nullptr;
	int playerLife = 8;
	//天球
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;
	Model* obModel = nullptr;
	
	//敵
	FrontEnemy* frontEnemy[11]{};
	LeftEnemy* leftEnemy[7]{};
	RightEnemy* rightEnemy[4]{};
	BackEnemy* backEnemy[2]{};
	EnemyBullet* enemyBullet = nullptr;
	//床
	TouchableObject* floor = nullptr;
	Model* floorModel = nullptr;
	//パーティクル
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	

	//ポジション
	XMFLOAT3 playerPos;
	XMFLOAT3 enePos;
	XMFLOAT3 bulPos;
	//スプライト
	XMFLOAT2 spPos;
	//敵のポジション
	XMFLOAT3 frontEnePos[11];
	XMFLOAT3 leftEnePos[7];
	XMFLOAT3 rightEnePos[4];
	XMFLOAT3 backEnePos[2];

	//タイマー
	int waitTimer = 0;
	//フラグ関係
	bool bulFlag = true;
	bool waitFlag = false;
	bool lifeFlag = false;
	int fEneFlag = 0;
	int lEneFlag = 0;
	int rEneFlag = 0;
	int bEneFlag = 0;
	// カメラ関係
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
};