#include "BackEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

BackEnemy::BackEnemy() :BackEnemy(Model::CreateFromOBJ("yellowSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0,0,0 });
}

BackEnemy::~BackEnemy()
{
}

bool BackEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	//弾の発射間隔
	AccessPhase(kShootInterval);
	return true;
}

void BackEnemy::Update()
{
	if (alive) {
		//敵の登場関数
		appearance();
		//敵の登場が終わったら弾を発射する
		if (!appFlag) {
			Shoot(position, kShootInterval);

		}
		object->SetPosition(position);
	}

	object->Update();
}

void BackEnemy::appearance()
{
	//扉の後ろから登場する時の処理
	position.z += moveZ;
	if (position.z >= -50) {
		moveZ = 0;
		appFlag = false;
	}
}

void BackEnemy::OnCollision()
{
	CreateParticle();
	life--;
	if (life <= 0) {
		alive = false;
	}
}

XMVECTOR BackEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}