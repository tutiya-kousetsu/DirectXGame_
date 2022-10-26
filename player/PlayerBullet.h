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

	void Initialize();

	void Update() override;

	//描画
	void Draw() override;

	inline const XMFLOAT3& GetVelocity() { return velocity; }
	inline void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	//弾が消える時間
	UINT disappearTime = 60 * 2;

	UINT frameNum = 0;
	//Model* bulletModel = nullptr;

private:
	Model* bulletModel = nullptr;
	Object3d* bulletObj = nullptr;
	DirectX::XMFLOAT3 velocity;
};

