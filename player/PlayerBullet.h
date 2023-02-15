#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public Object3d
{
public:
	PlayerBullet* Create(Model* model = nullptr);
public:
	//�G�C���A�X�A�֐��̌p���Ȃ�
	//using GameObject::GameObject;

	//�R���X�g���N�^
	//PlayerBullet();

	~PlayerBullet();

	bool Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//�`��
	void Draw() override;
	
	//const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
	//void SetPos(const DirectX::XMFLOAT3& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

	//�e�������鎞��
	UINT disappearTime = 50;
	
	UINT frameNum = 0;
	UINT power = 1;
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	XMVECTOR velocity;
	// ���x
	DirectX::XMFLOAT3 vel;
	//std::unique_ptr<Object3d> object;
	bool alive = true;
};

