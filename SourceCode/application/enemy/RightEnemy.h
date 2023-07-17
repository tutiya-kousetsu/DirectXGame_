#pragma once
#include "EnemyBullet.h"
#include "BaseEnemy.h"
#include "Audio.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

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

	//void OnCollision();
	//描画
	//void Draw() override;

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	ParticleManager* particleMan = nullptr;
	std::list < std::unique_ptr<EnemyBullet>> bullets;
	Audio* audio = nullptr;
};

