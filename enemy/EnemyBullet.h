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

	void Initialize(XMFLOAT3 pos, const XMVECTOR& vel);

	void Update() override;

	//描画
	void Draw() override;

	const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
  	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	// 速度を設定
	//inline void SetVel(const DirectX::XMFLOAT3& vel) { this->velocity = vel; }
	// 速度を取得
	//inline const DirectX::XMFLOAT3& GetVel() { return velocity; }

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 60 * 5.f;

	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
};

