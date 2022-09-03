#pragma once
#include "Object3d.h"
#include"Input.h"
using namespace DirectX;

class Player
{
public:
	//コンストラクタ
	Player();

	//デストラクタ
	~Player();

	//初期化
	void Initialize(Input* input);

	//更新
	void Update();

	//描画
	void Draw();

	XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	//WinApp* winApp = nullptr;
	Input* input = nullptr;
};

