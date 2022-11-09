#pragma once
#include "GameObject.h"
#include "Object3d.h"
#include "EnemyBullet.h"
#include <memory>
#include <list>

using namespace DirectX;

class Enemy : public GameObject
{
public:
	using GameObject::GameObject;

	//コンストラクタ
	Enemy();

	~Enemy();

	//初期化
	void Initialize();

	//更新
	void Update() override;

	void UpdateAliveFlag();

	//描画
	void Draw() override;

	void Shoot();

	//衝突時に呼び出される関数
	void OnCollision();

	//inline XMFLOAT3 GetPosition() { return enemyObj->GetPosition(); }
	//XMFLOAT3 frameGetPos() { return frameObj->GetPosition(); }

	//弾リスト取得
	//const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullet; }

public:
	int aliveFlag = 1;
	//衝突した処理
	void Hit() { aliveFlag = 0; }
	
	inline bool GetFlag() { return aliveFlag; }

	static const int kShootInterval = 60;

	//弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();
private:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	//Model* enemyModel = nullptr;
	//Model* frameModel = nullptr;
	//Object3d* enemyObj = nullptr;
	//Object3d* frameObj = nullptr;
	XMFLOAT3 position; 
	std::list<std::unique_ptr<Object3d>> enemy;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	//std::unique_ptr<Object3d>enemyObj;
};

