#pragma once
#include "Enemy.h"

class Player;

class LeftEnemy :public Enemy
{
public:
	LeftEnemy();

	//������
	void Initialize();

	//�X�V
	void Update() override;

	//void Draw() override;

	void LeftShoot();

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
	float move = 0.2f;

};

