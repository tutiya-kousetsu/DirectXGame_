#pragma once
#include "Enemy.h"
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
using namespace DirectX;

class EnemyBullet : public GameObject
{
public:
	//コンストラクタ
	EnemyBullet();

	//デストラクタ
	~EnemyBullet();

	void Initialize(Input* input, Enemy* enemy);

	void Update();

	void Draw();

	inline XMFLOAT3 GetPosition() { return enemyBulletObj->GetPosition(); }

	int aliveFlag = 0;

	int GetFlag() { return aliveFlag; }

	void Hit() { aliveFlag = 0; }

private:
	Object3d* enemyBulletObj = nullptr;
	Model* enemyBulletModel = nullptr;
	Enemy* enemy = nullptr;
	Input* input = nullptr;
};

