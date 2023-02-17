#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class PlayerBullet : public Object3d
{
public:
	PlayerBullet* Create(Model* model = nullptr);
public:
	//エイリアス、関数の継承など
	//using GameObject::GameObject;

	//コンストラクタ
	//PlayerBullet();

	~PlayerBullet();

	bool Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//描画
	void Draw() override;
	
	//const DirectX::XMFLOAT3& GetPos() const { return object->GetPosition(); }
	//void SetPos(const DirectX::XMFLOAT3& pos) { this->position = pos; }

	inline bool DisappearFlag() { return alive = false; }

	//衝突時に呼び出される関数
	void OnCollision();

	//弾が消える時間
	UINT disappearTime = 50;
	
	UINT frameNum = 0;
	UINT power = 1;
private:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	XMVECTOR velocity;
	// 速度
<<<<<<< HEAD
	XMFLOAT3 vel;
	std::unique_ptr<GameObject> object;
	bool alive = true;
};
=======
	DirectX::XMFLOAT3 vel;
	//std::unique_ptr<Object3d> object;
	bool alive = true;
};

>>>>>>> e334446c2f64c32fc7d1d6fa98b0c179ee443e0e
