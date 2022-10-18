#pragma once
#include "Object3d.h"

using namespace DirectX;
using namespace std;

class Enemy : public Object3d
{
private:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMFLOAT3;


public:
	//�R���X�g���N�^
	Enemy();

	//�f�X�g���N�^
	~Enemy();

	//������
	bool Initialize() override;

	//�X�V
	void Update() override;

	void OnCollision(const CollisionInfo& info) override;

	//�`��
	void Draw() override;

	//inline XMFLOAT3 GetPosition() { return enemyObj[0]->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj[0]->GetPosition(); }

public:
	//int aliveFlag = true;
	int frameFlag = 0;
	//�Փ˂�������
	//void Hit() { aliveFlag = 0; }
	
	//bool GetFlag() { return aliveFlag; }

	bool aliveFlag = true;

	void Hit() { aliveFlag = false; }

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

