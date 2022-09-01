#pragma once
#include "Object3d.h"

using namespace DirectX;

class Enemy
{
public:
	//コンストラクタ
	Enemy();

	//デストラクタ
	~Enemy();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	XMFLOAT3 GetPosition() { return enemyObj[0]->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj[0]->GetPosition(); }

	int aliveFlag[3] = {true, true, true};
	
	//衝突した処理
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

