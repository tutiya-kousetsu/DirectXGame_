#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>

using namespace DirectX;

class Player : public GameObject
{
public:
	using GameObject::GameObject;

	//�R���X�g���N�^
	Player();
	//�X�V
	void Update() override;

	//�ړ�
	void move(float speed = 0.2f);

	//�W�����v
	void jump();

	//�`��
	void Draw() override;

	//�U��
	void Shoot();

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e���X�g�擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	XMFLOAT3 position;
	XMFLOAT3 bulPos;
	// �W�����v
	bool jumpFlag = false;
	bool secondFlag = false;
	int jumpCount = 2;
	float g = 0.25;//�d��
	float jumpSpeed = 0;

};

