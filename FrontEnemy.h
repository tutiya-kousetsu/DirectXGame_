#pragma once
#include "GameObject.h"
#include "Enemy.h"

class Player;

class FrontEnemy : public Enemy
{
public:
	using Enemy::Enemy;

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