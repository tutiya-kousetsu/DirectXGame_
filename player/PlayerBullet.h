#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include "PlayerBulletObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public PlayerBulletObject
{
public:
	//エイリアス、関数の継承など
	//using GameObject::GameObject;

	//コンストラクタ
	//PlayerBullet();

	~PlayerBullet();

	void Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//描画
	void Draw();

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;
	UINT power = 1;
private:
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	std::unique_ptr<PlayerBulletObject> object;
	//int power = 1;
};

