#pragma once
#include "Enemy.h"

class Player;

class FrontEnemy : public Enemy
{
public:
	FrontEnemy();

	//������
	void Initialize();

	//�X�V
	void Update() override;
	//�o��
	void appearance();
	//�e�̏���
	void FrontShoot();

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
	float move = 0.2f;
	float moveY = 0.2f;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	bool shootFlag = false;
};