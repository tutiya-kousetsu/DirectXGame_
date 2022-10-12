#pragma once
#include "Object3d.h"

using namespace DirectX;
using namespace std;

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

	//inline XMFLOAT3 GetPosition() { return enemyObj[0]->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj[0]->GetPosition(); }

public:
	int aliveFlag = true;
	int frameFlag = 0;
	//�Փ˂�������
	void Hit() { aliveFlag = 0; }
	
	bool GetFlag() { return aliveFlag; }

	int flashingFlag = 0;
	int flashingEndFlag = 0;
	//vector<Object3d *> enemyObj;

private:
	int enemyTimer = 0;
	int frameTimer = 0;
	int flashingTimer = 0;
	Model* enemyModel = nullptr;
	Model* frameModel = nullptr;
	Object3d* enemyObj = nullptr;
	Object3d* frameObj = nullptr;
};

