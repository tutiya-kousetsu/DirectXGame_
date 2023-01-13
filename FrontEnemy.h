#pragma once
#include "GameObject.h"
#include "Enemy.h"

class FrontEnemy : public Enemy
{
public:
	FrontEnemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	void Draw() override;

public:

	static const int kShootInterval = 100;

	void AccessPhase();
	//void SetPlayer(Player* player) { this->player = player; }

private:
	Enemy* enemy = nullptr;
	//Player* player = nullptr;

};