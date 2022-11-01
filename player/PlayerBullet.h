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

	void Initialize(XMFLOAT3 pos);

	void Update() override;

	//�`��
	void Draw() override;

	inline const XMFLOAT3& GetVelocity() { return velocity; }
	inline void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	//�e�������鎞��
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;

private:
	XMFLOAT3 position;
	XMFLOAT3 velocity;
};

