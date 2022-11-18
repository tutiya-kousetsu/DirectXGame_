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

	void Initialize(const XMVECTOR& pos, const XMVECTOR& velocity);

	void Updata() override;

	//描画
	void Draw() override;
	
	const DirectX::XMVECTOR& GetPos() const { return object->GetPosition(); }
	void SetPos(const XMVECTOR& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 25;
	
	UINT frameNum = 0;
	UINT power = 1;
private:
	XMVECTOR position;
	XMVECTOR velocity;
};

