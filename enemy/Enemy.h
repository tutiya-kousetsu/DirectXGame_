#pragma once
#include "GameObject.h"
#include "Object3d.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

using namespace DirectX;
//player�̑O���錾
class Player;

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

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

	static const int kShootInterval = 70;

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
	XMFLOAT3 position; 
	XMVECTOR velocity;
	std::list<std::unique_ptr<Object3d>> enemy;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
};

