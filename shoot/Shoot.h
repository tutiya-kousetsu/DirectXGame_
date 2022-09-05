#pragma once
#include "Player.h"
#include "Input.h"
#include "Object3d.h"

using namespace DirectX;

class Shoot
{
public:
	//�R���X�g���N�^
	Shoot();

	//�f�X�g���N�^
	~Shoot();

	void Initialize(Input* input, Player* player);

	void Update();

	void Draw();

	XMFLOAT3 GetPosition() { return shootObj[0]->GetPosition(); }

	int aliveFlag = 0;

	int GetFlag() { return aliveFlag; }

	void Hit() { aliveFlag = 0; }

private:
	Object3d* shootObj[5] = {};
	Model* shootModel = nullptr;
	Player* player = nullptr;
	Input* input = nullptr;
};

