#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Object3d.h"
#include "GameObject.h"
#include "Audio.h"
#include <memory>
#include <list>

class FrontEnemy : public BaseEnemy
{
public:
	using BaseEnemy::BaseEnemy;

public:
	FrontEnemy();

	~FrontEnemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(XMFLOAT3 position);

	//更新
	void Update() override;

	//タイトルで使う何もしてこない敵用
	void TitleUpdate();

	//登場
	void appearance();

	void OnCollision();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:
	static const int kShootInterval = 100;
	XMFLOAT4 color = { 1.0f, 0, 0, 1 };
private:
	XMFLOAT3 position;
	XMVECTOR velocity;
	
	int life = 2;
	XMVECTOR worldPos{};
};