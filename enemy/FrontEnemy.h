#pragma once
#include "Enemy.h"

class Player;

class FrontEnemy : public Enemy
{
public:
	FrontEnemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	//void Draw() override;

	void FrontShoot();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

	void resurrection(int aliveCount);
public:

	static const int kShootInterval = 100;

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }

private:
	Enemy* enemy = nullptr;
	Player* player = nullptr;
	XMFLOAT3 position;
	XMVECTOR velocity;
	float move = 0.2f;
	float resurrectionTimer = 0;
	int aliveCount = 0;
};