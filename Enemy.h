#pragma once
#include "Object3d.h"

using namespace DirectX;

class Enemy
{
public:
	//�R���X�g���N�^
	Enemy();

	//�f�X�g���N�^
	~Enemy();

	//������
	void Initialize();

	//�X�V
	void Update();

	//�`��
	void Draw();

	XMFLOAT3 GetPosition() { return enemyObj[0]->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj[0]->GetPosition(); }

	int aliveFlag[3] = {true, true, true};
	
	//�Փ˂�������
	void Hit() { aliveFlag[0] = 0; }

	bool GetFlag() { return aliveFlag[0]; }
private:
	int enemyTimer = 0;
	int frameTimer = 0;
	int frameFlag = 0;
	Model* enemyModel = nullptr;
	Model* frameModel = nullptr;
	Object3d* enemyObj[3] = {};
	Object3d* frameObj[3] = {};
};

