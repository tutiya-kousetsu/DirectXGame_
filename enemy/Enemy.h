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

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

public://�T�u�N���X
	struct frontEnemy {

	};

	struct leftEnemy {

	};

	struct rightEnemy {

	};

	struct backEnemy {

	};

public:
	//�R���X�g���N�^
	Enemy();

	~Enemy();

	//������
	void Initialize();

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	void Shoot();

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

public:

	static const int kShootInterval = 120;

	//�e���X�g�擾
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();

	void SetPlayer(Player* player) { this->player = player; }

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

private:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	Player* player = nullptr;
	Line* line = nullptr;
	XMFLOAT3 position;
	XMFLOAT3 linePos;
	XMVECTOR velocity;
	std::list<std::unique_ptr<Object3d>> enemy;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 3;
};

