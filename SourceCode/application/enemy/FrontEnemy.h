#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Object3d.h"
#include "GameObject.h"
#include "Audio.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

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

	//void OnCollision();

	//�`��
	//void Draw() override;

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//�e���X�g�擾
	//const std::list < std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	bool bulFlag = true;
	int life = 2;
	XMVECTOR worldPos{};
	float move = 0.2f;
	float moveY = 0.2f;
	float moveZ = 0.05f;
	ParticleManager* particleMan = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	Audio* audio = nullptr;
};