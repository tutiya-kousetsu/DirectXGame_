#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public GameObject
{
public:
	//エイリアス、関数の継承など
	using GameObject::GameObject;

	//コンストラクタ
	PlayerBullet();

	void Initialize(XMFLOAT3 pos);

	void Update() override;

	//描画
	void Draw() override;

	inline const XMFLOAT3& GetVelocity() { return velocity; }
	inline void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	//弾が消える時間
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;

private:
	XMFLOAT3 position;
	XMFLOAT3 velocity;
};

