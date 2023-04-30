#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
//using namespace DirectX;

class PlayerBullet : public GameObject
{
public:
	//エイリアス、関数の継承など
	using GameObject::GameObject;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMVECTOR = DirectX::XMVECTOR;

	//コンストラクタ
	PlayerBullet();

	~PlayerBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//描画
	void Draw();

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	int32_t disappearTime = 60 * 2;

	int32_t frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
};

