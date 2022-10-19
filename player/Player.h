#pragma once
#include "Object3d.h"
#include"Input.h"
using namespace DirectX;

//�O���錾...���O�����؂�Ă��� = �����������Ȃ��ꍇ�g��
class Shoot;
class Enemy;

class Player: public Object3d
{
private:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMFLOAT3;
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
	void Update() override;

	//�ړ�
	void Move();

	//�`��
	void Draw() override;

	//setter
	void SetPosition(XMFLOAT3 position) { this->playerObj = playerObj; }

	//getter
	//inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

	UINT playerHp = 5;

private:
	Model* playerModel = nullptr;
	Model* shootModel = nullptr;
	std::vector<Shoot> bullet;
	Object3d* playerObj = nullptr;
	Object3d* shootObj = nullptr;
	Enemy* enemy[4] = {};

 	Input* input = nullptr;
};

