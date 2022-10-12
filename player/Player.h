#pragma once
#include "Object3d.h"
#include"Input.h"
#include "Enemy.h"
using namespace DirectX;

class Player: public Object3d
{
private:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMMATRIX;
public:
	/// <summary>
	/// 3Dオブジェクトの生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);

public:
	//コンストラクタ
	Player();

	//デストラクタ
	~Player();

	//初期化
	bool Initialize() override;

	//更新
	void Update(Shoot* shootPos) override;

	void OnCollision(const CollisionInfo& info) override;

	//描画
	void Draw() override;

	//setter
	void SetPosition(XMFLOAT3 position) { this->playerObj = playerObj; }

	//getter
	//inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

private:
	Model* playerModel = nullptr;

	Object3d* playerObj = nullptr;

	Enemy* enemy[4] = {};

 	Input* input = nullptr;
};

