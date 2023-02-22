#pragma once
#include "EnemyBullet.h"
#include "EnemyObject.h"
class Player;
class ParticleManager;

class BackEnemy :public EnemyObject
{
public:

	~BackEnemy();

	//初期化
	bool Initialize();

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

	static const int kShootInterval = 100;
	//弾リスト取得
	const std::unique_ptr<EnemyBullet>& GetBullet() { return bullets; }

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	float bMoveY = 0.2f;

	ParticleManager* particleMan = nullptr;
	std::unique_ptr<EnemyBullet> bullets;
	std::unique_ptr<EnemyObject> object;
};

