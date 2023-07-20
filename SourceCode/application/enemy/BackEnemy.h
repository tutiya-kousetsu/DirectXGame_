#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "GameObject.h"
#include "Audio.h"
#include <memory>
#include <list>

class BackEnemy :public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	BackEnemy();

	~BackEnemy();

	//初期化
	bool Initialize(XMFLOAT3 position);

	//更新
	void Update() override;

	//登場
	void appearance();

	void OnCollision();
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 150;

private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	XMFLOAT4 color = { 1, 1, 0, 1 };
	int life = 2;
};

