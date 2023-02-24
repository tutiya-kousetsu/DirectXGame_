#pragma once
#include "EnemyBullet.h"
#include "Object3d.h"]
#include "GameObject.h"
#include "EnemyObject.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class FrontEnemy : public GameObject
{
public:
	using GameObject::GameObject;

public:
	FrontEnemy();

	~FrontEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();


	//�X�V
	void Update();
	//�o��
	void appearance();
	//�e�̏���
	void FrontShoot();

	void Shoot();

	void OnCollision();

	//�`��
	void Draw();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//�e���X�g�擾
	const std::list < std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	ParticleManager* particleMan = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
};