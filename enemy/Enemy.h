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

	inline XMFLOAT3 GetPosition() { return enemyObj->GetPosition(); }
	XMFLOAT3 frameGetPos() { return frameObj->GetPosition(); }

public:
	int aliveFlag = 1;
	int frameFlag = 0;
	//�Փ˂�������
	void Hit() { aliveFlag = 0; }
	
	bool GetFlag() { return aliveFlag; }

	int flashingFlag = 0;
	int flashingEndFlag = 0;
	UINT clearFlag = 12;
private:
	int enemyTimer = 0;
	int frameTimer = 0;
	int flashingTimer = 0;
	Model* enemyModel = nullptr;
	Model* frameModel = nullptr;
	Object3d* enemyObj = nullptr;
	Object3d* frameObj = nullptr;
	XMFLOAT3 position;
};

