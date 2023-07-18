#include "BackEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

BackEnemy::BackEnemy() :BackEnemy(Model::CreateFromOBJ("yellowSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0,0,0 });
	sanderObj = Object3d::Create();
	sanderObj->SetModel(Model::CreateFromOBJ("sander"));
}

BackEnemy::~BackEnemy()
{
}

bool BackEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	sanderObj->SetPosition(position);
	AccessPhase(kShootInterval);
	return true;
}

void BackEnemy::Update()
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
		sanderObj->SetPosition(position);
	}

	object->Update();
	sanderObj->Update();
}

void BackEnemy::appearance()
{
	position.z += moveZ;
	if (position.z >= -50) {
		moveZ = 0;
		appFlag = false;
	}
}

void BackEnemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		sanderObj->Draw();
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