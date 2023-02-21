#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include "PlayerBulletObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public PlayerBulletObject
{
public:
	//�G�C���A�X�A�֐��̌p���Ȃ�
	//using GameObject::GameObject;

	//�R���X�g���N�^
	//PlayerBullet();

	~PlayerBullet();

	void Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//�`��
	void Draw();

	//�Փˎ��ɌĂяo�����֐�
	void OnCollision();

public:
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	//�e�������鎞��
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;
	UINT power = 1;
private:
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	std::unique_ptr<PlayerBulletObject> object;
	//int power = 1;
};

