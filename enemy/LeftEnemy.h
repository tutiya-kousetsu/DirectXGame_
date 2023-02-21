#pragma once
#include "EnemyBullet.h"

class Player;
class ParticleManager;

class LeftEnemy :public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static LeftEnemy* Create(Model* model = nullptr);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	~LeftEnemy();

	//更新
	void Update() override;

	//登場
	void appearance();

	void LeftShoot();

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

	int32_t shootTimer = 0;
	ParticleManager* particleMan = nullptr;
	std::unique_ptr<EnemyBullet> bullets;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	bool appFlag = true;
	int alive = true;
};

