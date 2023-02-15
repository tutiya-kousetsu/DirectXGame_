#pragma once
#include "EnemyBullet.h"

class Player;
class ParticleManager;

class BackEnemy :public Object3d
{
public:
	static BackEnemy* Create(Model* model = nullptr);

public:

	~BackEnemy();

	//������
	bool Initialize(Model* model);

	//�X�V
	void Update() override;

	//�o��
	void appearance();

	void BackShoot();

	void Shoot();

	//�`��
	void Draw();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//�e���X�g�擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float move = 0.2f;
	float bMoveY = 0.2f;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	int32_t shootTimer = 0;
	ParticleManager* particleMan = nullptr;
	//Line* line = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 2;
	float moveY = 0.2f;
	bool appFlag = true;
	int alive = true;
};

