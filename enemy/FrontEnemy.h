#pragma once
#include "EnemyBullet.h"
#include "Object3d.h"
#include "GameObject.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class FrontEnemy : public GameObject
{
public:
	using GameObject::GameObject;

public:
	FrontEnemy();

	~FrontEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	//更新
	void Update();
	//登場
	void appearance();
	//弾の処理
	void FrontShoot();

	void Shoot();

	void OnCollision();

	//描画
	void Draw();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//弾リスト取得
	const std::list < std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }


	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	float moveZ = 0.05f;
	ParticleManager* particleMan = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
};