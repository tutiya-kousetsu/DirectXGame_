#include "LeftEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

LeftEnemy::LeftEnemy() :LeftEnemy(Model::CreateFromOBJ("greenSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
}

LeftEnemy::~LeftEnemy()
{
}

bool LeftEnemy::Initialize(XMFLOAT3 position, XMFLOAT3 rotation)
{
	this->position = position;
	this->rotation = rotation;
	SetRotation(rotation);
	//弾の発射間隔
	AccessPhase(kShootInterval);
	return true;
}

void LeftEnemy::Update()
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

void LeftEnemy::OnCollision()
{
	CreateParticle();
	life--;
	if (life <= 0) {
		alive = false;
	}
}

void LeftEnemy::appearance()
{
	//扉の後ろから登場する時の処理
	if (position.x <= -50) {
		position.x += moveX;
		if (position.x >= -50) {
			moveX = 0;
			appFlag = false;
		}
	}
	if (position.z >= 50) {
		position.z -= moveZ;
		if (position.z <= 50) {
			moveZ = 0;
			appFlag = false;
		}
	}
}

XMVECTOR LeftEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}