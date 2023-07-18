#include "RightEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

RightEnemy::RightEnemy() :RightEnemy(Model::CreateFromOBJ("purpleSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0,270,0 });
}

RightEnemy::~RightEnemy()
{
}

bool RightEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	AccessPhase(kShootInterval);
	return true;
}

void RightEnemy::Update()
{
	//弾のフラグがfalseになったら削除する
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();

		if (!appFlag) {
			Shoot(position, kShootInterval);

		}
		object->SetPosition(position);
	}

	object->Update();
}



void RightEnemy::appearance()
{
	position.x -= moveX;
	if (position.x <= 50) {
		moveX = 0;
		appFlag = false;
	}
}

void RightEnemy::OnCollision()
{
	CreateParticle();
	life--;
	if (life <= 0) {
		alive = false;
	}
}

XMVECTOR RightEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}