#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>

class RightEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	RightEnemy();

	~RightEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 position);

	//更新
	void Update();

	//登場
	void appearance();

	void OnCollision();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	int life = 2;
};

