#pragma once
#include "EnemyBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class BackEnemy :public GameObject
{
public:
	using GameObject::GameObject;

public:
	BackEnemy();

	~BackEnemy();

	//初期化
	bool Initialize(XMFLOAT3 position);

	//更新
	void Update() override;

	//登場
	void appearance();

	void BackShoot();

	void Shoot();

	//描画
	void Draw();
	void OnCollision();
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 150;
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
	float bMoveY = 0.2f;

	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
};

