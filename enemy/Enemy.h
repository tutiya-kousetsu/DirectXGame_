#pragma once
#include "GameObject.h"
#include "Object3d.h"
#include "EnemyBullet.h"
#include "Line.h"
#include <memory>
#include <list>

using namespace DirectX;
//playerの前方宣言
class Player;
class ParticleManager;

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

public:
	//コンストラクタ
	Enemy();

	~Enemy();

	//描画
	void Draw() override;

	//衝突時に呼び出される関数
	void OnCollision();

public:

	static const int kShootInterval = 120;

	//弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

protected:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	Player* player = nullptr;
	ParticleManager* particleMan = nullptr;
	Line* line = nullptr;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 3;

};

