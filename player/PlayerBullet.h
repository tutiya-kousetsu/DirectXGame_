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
	
	const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
	void SetPos(const XMFLOAT3& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 60 * 1.5f;

	UINT frameNum = 0;
	UINT power = 1;
private:
	XMFLOAT3 position;
};

