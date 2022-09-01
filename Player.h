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

	//�`��
	void Draw();

	XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	//WinApp* winApp = nullptr;
	Input* input = nullptr;
};

