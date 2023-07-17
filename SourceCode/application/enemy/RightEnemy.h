#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

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

	//void OnCollision();
	//�`��
	//void Draw() override;

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
	Audio* audio = nullptr;
};

