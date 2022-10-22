#pragma once
#include "Object3d.h"
#include"Input.h"
using namespace DirectX;

class Player
{
public:
	//�R���X�g���N�^
	Player();

	//�f�X�g���N�^
	~Player();

	//������
	void Initialize(Input* input);

	//�X�V
	void Update();

	//�ړ�
	void move();

	//�`��
	void Draw();

	inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	inline XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	//WinApp* winApp = nullptr;
	Input* input = nullptr;

	// �W�����v
	bool jumpFlag = false;
	bool secondFlag = false;
	float g = 0.25;//�d��
	float jumpSpeed = 0;
};

