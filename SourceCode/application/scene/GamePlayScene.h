#pragma once

#include "BaseScene.h"
#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Fbx_Object3d.h"
#include "PostEffect.h"
#include "PlayerBullet.h"
#include "Tutorial.h"
#include "TitleScene.h"
#include "FollowingCamera.h"
#include "Input.h"
#include "Obstacle.h"
#include "FrontEnemy.h"
#include "LeftEnemy.h"
#include "RightEnemy.h"
#include "BackEnemy.h"
#include "Wall.h"
#include "Door.h"
#include "Player.h"
#include "TouchableObject.h"
#include "Phase.h"
#include "PlayerLife.h"
#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Shake.h"
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
	std::unique_ptr<Sprite> clear;
	//表示されている間の秒数
	int32_t damageTime = 20;
	int phaseCount = 0;
	std::unique_ptr<Phase> phase;

	//イーズイン用frame
	float inFrame = 0.f;
	float easFrame = 0.f;
	
	std::unique_ptr<Sprite> spriteBG;
	// カメラ
	std::unique_ptr<FollowingCamera> camera;
	std::unique_ptr<DebugCamera> debugCam;
	Camera* nowCamera = nullptr;

	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	std::unique_ptr<PostEffect> postEffect;
	
	//ゲームが終わった時に暗くするやつの半径
	float endEfRadius = 1000;
	
	//障害物
	std::list<std::unique_ptr<Obstacle>> obstacles;
	std::stringstream obstaclePopCom;
	//プレイヤー
	std::unique_ptr<Player> player;
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
	//床
	std::unique_ptr<TouchableObject> floor;

	//パーティクル
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	
	BaseCollider* collider = nullptr;

	//ドア
	std::unique_ptr<Door> door;
	//壁
	std::list<std::unique_ptr<Wall>> walls;
	std::stringstream wallPopCom;
	//スプライト
	XMFLOAT2 phasePos;
	XMFLOAT2 clearPos;
	XMFLOAT2 spPos;
	//ポジション
	//ドア
	XMFLOAT3 doorPos;
	//自機
	XMFLOAT3 playerPos;
	//自機のスケール
	XMFLOAT3 playerScale;
	//ローテーション
	XMFLOAT3 doorRot;
	XMFLOAT3 playerRot;
	//スプライトのカラー
	XMFLOAT4 damageColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	XMFLOAT4 color;

	//タイマー
	int32_t wait = 0;
	int32_t clearTime = 0;
	int32_t landTime = 0;
	int32_t sceneTime = 0;
	int32_t numbTime = 0;
	int32_t downTime = 0;
	//フラグ
	bool bulFlag = true;//弾のフラグ
	bool waitFlag = false;//待ってるときのフラグ
	bool clearFlag = false;
	bool clearTFlag = false;
	bool damageFlag1 = false;//敵弾に当たった時
	bool damageFlag2 = false;//敵弾に当たった時
	bool damageFlag3 = false;//敵弾に当たった時
	bool damageFlag4 = false;//敵弾に当たった時
	bool rayFlag = false;
	bool endFlag = false;//ゲームが終わったか
	bool landFlag = false;//着地しているか
	bool numbFlag = false;
	bool hpFlag = false;
	bool phaseFlag = false;
	bool upFlag = false;
	//フェーズ
	//前の敵
	int fEnePhase = 0;
	int fWaitPhase = 0;
	//左の敵
	int lEnePhase = 0;
	int lWaitPhase = 0;
	//右の敵
	int rEnePhase = 0;
	int rWaitPhase = 0;
	//後ろの敵
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