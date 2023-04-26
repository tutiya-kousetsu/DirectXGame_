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
#include "Tutorial.h"
#include "GameClear.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "obstacle/Obstacle.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "wall/Wall.h"
#include "door/Door.h"
#include "Player.h"
#include "TouchableObject.h"
#include "Phase.h"
#include "PlayerLife.h"
#include "CollisionManager.h"
#include <memory>
#include <list>
#include <sstream>

//前方宣言
class ParticleManager;
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

	//ドアを敵のフェーズに合わせて動かす
	//void DoorMove();

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

	//敵の発生データの読み込み
	void LoadEnemyPopData();
	//敵配置のコマンド更新
	void UpdataEnemyPopCommand();

	//壁の発生データの読み込み
	void LoadWallPopData();
	//壁配置のコマンド更新
	void UpdataWallPopCommand();

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
	std::unique_ptr<Sprite> sprite;
	//HP
	std::unique_ptr<Sprite> LifeSprite;
	std::unique_ptr<Sprite> LifeSprite2;
	std::unique_ptr<Sprite> LifeSprite3;
	std::unique_ptr<Sprite> LifeSprite4;
	std::unique_ptr<Sprite> LifeSprite5;
	std::unique_ptr<Sprite> LifeSprite6;
	std::unique_ptr<Sprite> LifeSprite7;
	std::unique_ptr<Sprite> LifeSprite8;
	std::unique_ptr<Sprite> alignment;
	std::unique_ptr<Sprite> onAlignment;
	std::unique_ptr<Sprite> damage;
	XMFLOAT4 color;
	
	bool damageFlag1 = false;
	bool damageFlag2 = false;
	bool damageFlag3 = false;
	bool damageFlag4 = false;

	int32_t damageTime = 20;
	int phaseCount = 0;
	XMFLOAT2 phasePos;
	XMFLOAT4 damageColor = {1.0f, 1.0f, 1.0f, 1.0f};
	std::unique_ptr<Phase> phase;

	//イーズイン用frame
	float inFrame = 0.f;
	//イーズアウト用frame
	float outFrame[8]{};

	bool frameFlag = false;
	std::unique_ptr<Sprite> spriteBG;
	// カメラ
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	std::unique_ptr<PostEffect> postEffect;

	//int32_t endEfTime = 0;
	bool endFlag = false;
	float endEfRadius = 1000;
	
	//障害物
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;
	//プレイヤー
	std::unique_ptr<Player> player;
	XMFLOAT3 playerPos;
	XMFLOAT3 playerRot;
	XMFLOAT3 playerScale;
	int life = 0;
	std::unique_ptr<PlayerLife> playerLife;
	//天球
	std::unique_ptr<Object3d> skyObj;
	std::unique_ptr<Object3d> object;
	//敵
	std::list < std::unique_ptr<FrontEnemy>> frontEnemy;
	std::list < std::unique_ptr<LeftEnemy>> leftEnemy;
	std::list < std::unique_ptr<RightEnemy>> rightEnemy;
	std::list < std::unique_ptr<BackEnemy>> backEnemy;

	std::stringstream enemyPopCom;
	std::stringstream leftPopCom;
	std::stringstream rightPopCom;
	std::stringstream backPopCom;
	//床
	std::unique_ptr<TouchableObject> floor;

	//パーティクル
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	//ドア
	//Door* door[8]{};
	std::unique_ptr<Door> door;
	XMFLOAT3 doorPos;
	XMFLOAT3 doorRot;

	//壁
	std::list<std::unique_ptr<Wall>> walls;
	std::stringstream wallPopCom;
	//ポジション
	
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
	int32_t wait = 0;
	//フラグ
	bool bulFlag = true;
	bool waitFlag = false;
	bool timeFlag = false;
	bool lifeFlag = false;
	//フェーズ
	int fEnePhase = 0;
	int fWaitPhase = 0;
	int lEnePhase = 0;
	int lWaitPhase = 0;
	int rEnePhase = 0;
	int rWaitPhase = 0;
	int bEnePhase = 0;
	int bWaitPhase = 0;
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