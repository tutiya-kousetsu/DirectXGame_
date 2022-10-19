#pragma once
#include "Object3d.h"
#include"Input.h"
using namespace DirectX;

//前方宣言...名前だけ借りてくる = 処理を書かない場合使う
class Shoot;
class Enemy;

class Player: public Object3d
{
private:
	using XMMATRIX = DirectX::XMMATRIX;
	using XMFLOAT3 = DirectX::XMFLOAT3;
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
	void Update() override;

	//移動
	void Move();

	//描画
	void Draw() override;

	//setter
	void SetPosition(XMFLOAT3 position) { this->playerObj = playerObj; }

	//getter
	//inline XMFLOAT3 GetPosition() { return playerObj->GetPosition(); }
	//XMFLOAT3 GetRotation() { return playerObj->GetRotation(); }

	UINT playerHp = 5;

private:
	Model* playerModel = nullptr;
	Model* shootModel = nullptr;
	std::vector<Shoot> bullet;
	Object3d* playerObj = nullptr;
	Object3d* shootObj = nullptr;
	Enemy* enemy[4] = {};

 	Input* input = nullptr;
};

