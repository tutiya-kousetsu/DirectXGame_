#pragma once
#include "Input.h"
#include "Object3d.h"
#include "GameObject.h"
#include "EnemyBulletObject.h"
#include <DirectXMath.h>
using namespace DirectX;

class EnemyBullet : public EnemyBulletObject
{
public:

	~EnemyBullet();

	void Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel);

	void Update();

	//•`‰æ
	void Draw();

	//Õ“Ë‚ÉŒÄ‚Ño‚³‚ê‚éŠÖ”
	void OnCollision();
public:
	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	//’e‚ªÁ‚¦‚éŠÔ
	UINT disappearTime = 60 * 5;

	UINT frameNum = 0;
	UINT power = 1;
private:
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	std::unique_ptr<EnemyBulletObject> object;
};

