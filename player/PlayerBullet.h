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

	void Initialize();

	void Update() override;

	//�`��
	void Draw() override;

	inline const XMFLOAT3& GetVelocity() { return velocity; }
	inline void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	//�e�������鎞��
	UINT disappearTime = 60 * 2;

	UINT frameNum = 0;
	//Model* bulletModel = nullptr;

private:
	Model* bulletModel = nullptr;
	Object3d* bulletObj = nullptr;
	DirectX::XMFLOAT3 velocity;
};

