#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>


class LeftEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	LeftEnemy();

	~LeftEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 position, XMFLOAT3 rotation);

	//更新
	void Update();

	//登場
	void appearance();

	void OnCollision();
	//描画
	//void Draw() override;

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:
	static const int kShootInterval = 50;
private:
	XMFLOAT3 position;
	XMFLOAT3 rotation;
	XMVECTOR velocity;
	int life = 3;
};

