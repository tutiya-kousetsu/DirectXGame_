#pragma once
#include "GameObject.h"
#include "Object3d.h"
#include "EnemyBullet.h"
#include "Line.h"
#include <memory>
#include <list>

using namespace DirectX;
//player�̑O���錾
class Player;
class ParticleManager;

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

public:
	//�R���X�g���N�^
	Enemy();

	~Enemy();

	//�`��
	void Draw() override;

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

public:

	static const int kShootInterval = 120;

	//�e���X�g�擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

protected:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	Player* player = nullptr;
	ParticleManager* particleMan = nullptr;
	Line* line = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 3;

};

