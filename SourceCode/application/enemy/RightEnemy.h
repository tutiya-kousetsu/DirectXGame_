#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>

class RightEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	RightEnemy();

	~RightEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 position);

	//�X�V
	void Update();

	//�o��
	void appearance();

	void OnCollision();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	int life = 2;
};

