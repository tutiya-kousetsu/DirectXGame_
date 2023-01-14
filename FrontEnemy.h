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

	//������
	void Initialize();

	//�X�V
	void Update() override;

	//void Draw() override;

	void FrontShoot();

	//���[���h���W���擾
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