#pragma once
#include "EnemyBullet.h"
#include "EnemyObject.h"

class Player;
class ParticleManager;

class LeftEnemy :public EnemyObject
{
public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	~LeftEnemy();

	//�X�V
	void Update();

	//�o��
	void appearance();

	void LeftShoot();

	void Shoot();
	void OnCollision();
	//�`��
	void Draw();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//�e���X�g�擾
	const std::unique_ptr<EnemyBullet>& GetBullet() { return bullets; }

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	ParticleManager* particleMan = nullptr;
	std::unique_ptr<EnemyBullet> bullets;
	std::unique_ptr<EnemyObject> object;
};

