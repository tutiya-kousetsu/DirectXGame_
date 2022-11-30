#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBullet : public GameObject
{
public:
	//�G�C���A�X�A�֐��̌p���Ȃ�
	using GameObject::GameObject;

	//�R���X�g���N�^
	EnemyBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update() override;

	//�`��
	void Draw() override;

	const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
  	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e�������鎞��
	UINT disappearTime = 60 * 5.f;

	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
};

