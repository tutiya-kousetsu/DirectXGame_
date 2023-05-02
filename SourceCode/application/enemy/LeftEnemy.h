#pragma once
#include "EnemyBullet.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class LeftEnemy :public GameObject
{
public:
	using GameObject::GameObject;

public:
	LeftEnemy();

	~LeftEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 position);

	//更新
	void Update();

	//登場
	void appearance();

	void LeftShoot();

	void Shoot();
	void OnCollision();
	//描画
	void Draw();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 50;
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

	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
};

