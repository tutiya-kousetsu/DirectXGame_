#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

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

	//void OnCollision();
	//�`��
	//void Draw() override;

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:
	static const int kShootInterval = 50;

private:
	Player* player = nullptr;
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	int life = 3;
	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
	Audio* audio = nullptr;
};

