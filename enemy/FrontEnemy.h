#pragma once
#include "Enemy.h"

class Player;

class FrontEnemy : public Enemy
{
public:
	FrontEnemy();

	~FrontEnemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;
	//登場
	void appearance();
	//弾の処理
	void FrontShoot();

	void Shoot();
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();


public:

	static const int kShootInterval = 100;

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }

private:
	Enemy* enemy = nullptr;
	Player* player = nullptr;
	XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
};