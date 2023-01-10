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

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

public://サブクラス
	struct frontEnemy {

	};

	struct leftEnemy {

	};

	struct rightEnemy {

	};

	struct backEnemy {

	};

public:
	//コンストラクタ
	Enemy();

	~Enemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	//描画
	void Draw() override;

	void Shoot();

	//衝突時に呼び出される関数
	void OnCollision();

public:

	static const int kShootInterval = 120;

	//弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();

	void SetPlayer(Player* player) { this->player = player; }

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

private:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	Player* player = nullptr;
	Line* line = nullptr;
	XMFLOAT3 position;
	XMFLOAT3 linePos;
	XMVECTOR velocity;
	std::list<std::unique_ptr<Object3d>> enemy;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;
	int life = 3;
};

