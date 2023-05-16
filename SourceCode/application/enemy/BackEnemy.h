#pragma once
#include "EnemyBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class BackEnemy :public GameObject
{
public:
	using GameObject::GameObject;

public:
	BackEnemy();

	~BackEnemy();

	//������
	bool Initialize(XMFLOAT3 position);

	//�X�V
	void Update() override;

	//�o��
	void appearance();

	void BackShoot();

	void Shoot();

	//�`��
	void Draw();
	void OnCollision();
	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 150;
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
	float bMoveY = 0.2f;

	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
};

