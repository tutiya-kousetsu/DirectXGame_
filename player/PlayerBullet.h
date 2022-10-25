#pragma once
#include "Player.h"
#include "Input.h"
#include "Object3d.h"

using namespace DirectX;

class PlayerBullet
{
public:
	//コンストラクタ
	PlayerBullet();

	//デストラクタ
	~PlayerBullet();

	void Initialize(Input* input, Player* player);

	void Update();

	void Draw();

	inline XMFLOAT3 GetPosition() { return playerBulletObj->GetPosition(); }

	int aliveFlag = 0;

	int GetFlag() { return aliveFlag; }

	void Hit() { aliveFlag = 0; }

private:
	Object3d* playerBulletObj = nullptr;
	Model* playerBulletModel = nullptr;
	Player* player = nullptr;
	Input* input = nullptr;
};

