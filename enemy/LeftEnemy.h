#pragma once
#include "Enemy.h"

class Player;

class LeftEnemy :public Enemy
{
public:
	LeftEnemy();

	~LeftEnemy();

	//������
	void Initialize();

	//�X�V
	void Update() override;

	//�o��
	void appearance();

	void LeftShoot();

	void Shoot();

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

