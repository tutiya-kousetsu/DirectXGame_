#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>


class LeftEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	LeftEnemy();

	~LeftEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 position, XMFLOAT3 rotation);

	//�X�V
	void Update();

	//�o��
	void appearance();

	void OnCollision();
	//�`��
	//void Draw() override;

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:
	static const int kShootInterval = 50;
private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMVECTOR velocity;
	int life = 3;
};

