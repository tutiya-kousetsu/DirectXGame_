#pragma once
#include "Enemy.h"
#include "EnemyObject.h"

class Player;

class BackEnemy :public EnemyObject
{
public:
	//BackEnemy();

	~BackEnemy();

	//初期化
	bool Initialize() override;

	//更新
	void Update() override;

	//登場
	void appearance();

	void BackShoot();

	void Shoot();

	//描画
	void Draw();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();
	//void SetPlayer(Player* player) { this->player = player; }
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	Enemy* enemy = nullptr;
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float move = 0.2f;
	float bMoveY = 0.2f;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	int32_t shootTimer = 0;
	ParticleManager* particleMan = nullptr;
	Line* line = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 2;
	float moveY = 0.2f;
	bool appFlag = true;
	std::unique_ptr<EnemyObject> object;
	int alive = true;
};

