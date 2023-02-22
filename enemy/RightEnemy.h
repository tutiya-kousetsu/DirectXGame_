#pragma once
#include "EnemyBullet.h"
#include "EnemyObject.h"
class Player;
class ParticleManager;

class RightEnemy :public EnemyObject
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	~RightEnemy();

	//更新
	void Update();

	//登場
	void appearance();

	void RightShoot();

	void Shoot();

	//描画
	void Draw();

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//弾リスト取得
	const std::unique_ptr<EnemyBullet>& GetBullet() { return bullets; }

	void AccessPhase();

	void SetPlayer(Player* player) { this->player = player; }
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;

	ParticleManager* particleMan = nullptr;
	std::unique_ptr<EnemyBullet> bullets;
	std::unique_ptr<EnemyObject> object;
};

