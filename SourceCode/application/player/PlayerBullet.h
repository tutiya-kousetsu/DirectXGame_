#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
//using namespace DirectX;

class PlayerBullet : public GameObject
{
public:
	//�G�C���A�X�A�֐��̌p���Ȃ�
	using GameObject::GameObject;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	//�R���X�g���N�^
	PlayerBullet();

	~PlayerBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//�`��
	void Draw();

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e�������鎞��
	int32_t disappearTime = 60 * 2;

	int32_t frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
};

