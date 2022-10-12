#pragma once
//#include "Player.h"
#include "Input.h"
#include "Object3d.h"

using namespace DirectX;
class Player;

class Shoot
{
public:
	//コンストラクタ
	Shoot();

	//デストラクタ
	~Shoot();

	void Initialize(Input* input, Player* player);

	void Update();

	void Draw();

	//inline XMFLOAT3 GetPosition() { return shootObj->GetPosition(); }

	XMFLOAT3 SetPosition(XMFLOAT3 position) { this->player = player; }

	int aliveFlag = 0;

	int GetFlag() { return aliveFlag; }

	void Hit() { aliveFlag = 0; }

private:
	Object3d* shootObj = nullptr;
	Model* shootModel = nullptr;
	Player* player = nullptr;
	Input* input = nullptr;
};

