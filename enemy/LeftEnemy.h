#pragma once
#include "EnemyBullet.h"

class Player;
class ParticleManager;

class LeftEnemy :public Object3d
{
public:
	/// <summary>
	/// �I�u�W�F�N�g����
	/// </summary>
	/// <returns></returns>
	static LeftEnemy* Create(Model* model = nullptr);

public:
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(Model* model);

	~LeftEnemy();

	//�X�V
	void Update() override;

	//�o��
	void appearance();

	void LeftShoot();

	void Shoot();

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

	int32_t shootTimer = 0;
	ParticleManager* particleMan = nullptr;
	std::unique_ptr<EnemyBullet> bullets;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	bool appFlag = true;
	int alive = true;
};

