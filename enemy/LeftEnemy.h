#pragma once
#include "Enemy.h"

class Player;

class LeftEnemy :public Enemy
{
public:
	LeftEnemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	//登場
	void appearance();

	void LeftShoot();

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
};

