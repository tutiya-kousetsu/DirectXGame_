#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Object3d.h"
#include "GameObject.h"
#include "Audio.h"
#include <memory>
#include <list>

class FrontEnemy : public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	FrontEnemy();

	~FrontEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 position);

	//�X�V
	void Update() override;

	//�^�C�g���Ŏg���������Ă��Ȃ��G�p
	void TitleUpdate();

	//�o��
	void appearance();

	void OnCollision();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:
	static const int kShootInterval = 100;
	XMFLOAT4 color = { 1.0f, 0, 0, 1 };
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	
	int life = 2;
	XMVECTOR worldPos{};
};