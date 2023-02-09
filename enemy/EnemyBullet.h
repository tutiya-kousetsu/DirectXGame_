#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBullet : public GameObject
{
public:
	//エイリアス、関数の継承など
	using GameObject::GameObject;

	//コンストラクタ
	EnemyBullet();

	~EnemyBullet();

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update() override;

	//描画
	void Draw() override;

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
};

