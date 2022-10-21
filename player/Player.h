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

	inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	inline XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	//WinApp* winApp = nullptr;
	Input* input = nullptr;

	// ジャンプ
	bool jumpFlag = false;
	float jumpSpead = 0;
	DirectX::XMFLOAT3 jumpStartPos{ 0.f,0.f,0.f };
};

