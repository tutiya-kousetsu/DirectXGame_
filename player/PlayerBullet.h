#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public GameObject
{
public:
	//�G�C���A�X�A�֐��̌p���Ȃ�
	using GameObject::GameObject;

	//�R���X�g���N�^
	PlayerBullet();

	void Initialize(const XMVECTOR& pos, const XMVECTOR& velocity);

	void Updata() override;

	//�`��
	void Draw() override;
	
	const DirectX::XMVECTOR& GetPos() const { return object->GetPosition(); }
	void SetPos(const XMVECTOR& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e�������鎞��
	UINT disappearTime = 25;
	
	UINT frameNum = 0;
	UINT power = 1;
private:
	XMVECTOR position;
	XMVECTOR velocity;
};

