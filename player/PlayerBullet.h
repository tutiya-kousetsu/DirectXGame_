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

	~PlayerBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update() override;

	//�`��
	void Draw() override;
	
	const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e�������鎞��
	UINT disappearTime = 50;
	
	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMVECTOR velocity;
	// ���x
	XMFLOAT3 vel;
};

