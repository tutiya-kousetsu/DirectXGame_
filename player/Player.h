#pragma once
#include "Object3d.h"
#include"Input.h"
#include "Enemy.h"
using namespace DirectX;

class Player: public Object3d
{
private:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMMATRIX;
public:
	/// <summary>
	/// 3D�I�u�W�F�N�g�̐���
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static Player* Create(Model* model = nullptr);

public:
	//�R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player();

	//������
	bool Initialize() override;

	//�X�V
	void Update(Shoot* shootPos) override;

	void OnCollision(const CollisionInfo& info) override;

	//�`��
	void Draw() override;

	//setter
	void SetPosition(XMFLOAT3 position) { this->playerObj = playerObj; }

	//getter
	//inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	Enemy* enemy[4] = {};

 	Input* input = nullptr;
};

