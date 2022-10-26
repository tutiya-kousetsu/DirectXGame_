#pragma once
#include "GameObject.h"
#include "Object3d.h"

using namespace DirectX;

class Enemy
{
public:
	//コンストラクタ
	Enemy();

	//初期化
	void Initialize();

	//更新
	void Update();

	void UpdateAliveFlag();

	//描画
	void Draw();

	inline XMFLOAT3 GetPosition() { return enemyObj->GetPosition(); }
	XMFLOAT3 frameGetPos() { return frameObj->GetPosition(); }

public:
	int aliveFlag = 1;
	int frameFlag = 0;
	//衝突した処理
	void Hit() { aliveFlag = 0; }
	
	bool GetFlag() { return aliveFlag; }
	bool GetFrameFlag() { return frameFlag; }
	bool flashingFlag = false;
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

	//std::unique_ptr<Object3d>enemyObj;
};

