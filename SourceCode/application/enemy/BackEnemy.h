#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "GameObject.h"
#include "Audio.h"
#include <memory>
#include <list>

class BackEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	BackEnemy();

	~BackEnemy();

	//������
	bool Initialize(XMFLOAT3 position);

	//�X�V
	void Update() override;

	//�o��
	void appearance();

	void OnCollision();
	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 150;

private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	XMFLOAT4 color = { 1, 1, 0, 1 };
	int life = 2;
};

