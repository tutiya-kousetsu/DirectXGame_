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

	inline XMFLOAT3 GetPosition() { return enemyObj[0]->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj[0]->GetPosition(); }

public:
	int aliveFlag[4] = {true, true, true, true};
	int frameFlag = 0;
	//衝突した処理
	void Hit() { aliveFlag[0] = 0; }
	
	bool GetFlag() { return aliveFlag[0]; }

	int flashingFlag = 0;
	int flashingEndFlag = 0;
	UINT clearFlag = 12;
private:
	int enemyTimer = 0;
	int frameTimer = 0;
	int flashingTimer = 0;
	Model* enemyModel = nullptr;
	Model* frameModel = nullptr;
	Object3d* enemyObj[4] = {};
	Object3d* frameObj[4] = {};
};

