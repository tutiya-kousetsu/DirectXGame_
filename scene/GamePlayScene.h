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

	void FrontColl();

	void LeftColl();

	void RightColl();

	void BackColl();

	void CheckAllCollision();
private:
	
	Sprite* sprite = nullptr;
	Sprite* LifeSprite = nullptr;
	Sprite* LifeSprite2 = nullptr;
	Sprite* LifeSprite3 = nullptr;
	Sprite* spriteBG = nullptr;
	// カメラ
	std::unique_ptr<FollowingCamera> camera;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	PostEffect* postEffect[2] = {};

	std::list<std::unique_ptr<Obstacle>> obstacles;
	//障害物発生のコマンド
	std::stringstream obstaclePopCom;
	Object3d* skyObj = nullptr;
	Model* skyModel = nullptr;
	Model* obModel = nullptr;
	//std::unique_ptr<PlayerBullet> playerBullet;
	Player* player = nullptr;
	//std::unique_ptr<Player> player;
	//Enemy* enemy = nullptr;
	FrontEnemy* frontEnemy[11]{};
	LeftEnemy* leftEnemy[7]{};
	RightEnemy* rightEnemy[4]{};
	BackEnemy* backEnemy[2]{};
	//std::list<std::unique_ptr<Enemy>> enemy;
	PlayerBullet* playerBullet = nullptr;
	EnemyBullet* enemyBullet = nullptr;
	TouchableObject* floor = nullptr;
	Model* floorModel = nullptr;
	ParticleManager* particleMan = nullptr;
	CollisionManager* collisionMan = nullptr;
	//Line* line = nullptr;
	XMFLOAT3 enePos;

	XMFLOAT3 playerPos;
	XMFLOAT2 spPos;
	XMFLOAT3 bulPos;
	int playerLife = 3;

	int gameScore = 0;
	float flagTimer = 0;
	float fEneTimer[3] = {};
	float waitTimer = 0;
	//フラグ関係
	bool bulFlag = true;
	bool posFlag = false;

	bool waitFlag = false;
	bool lifeFlag = false;
	int fEneFlag = 0;
	int lEneFlag = 0;
	int rEneFlag = 0;
	int bEneFlag = 0;
	int enemyScene = 0;
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