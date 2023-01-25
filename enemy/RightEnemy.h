#pragma once
#include "Enemy.h"

class Player;

class RightEnemy :public Enemy
{
public:
	RightEnemy();

	~RightEnemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	//登場
	void appearance();

	void RightShoot();

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

