#include "FrontEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"

FrontEnemy::FrontEnemy() :FrontEnemy(Model::CreateFromOBJ("squid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0, 180, 0 });
}

FrontEnemy::~FrontEnemy()
{
}

bool FrontEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	//弾の発射間隔
	AccessPhase(kShootInterval);
	return true;
}

void FrontEnemy::Update()
{
	color = object->GetColor();
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

void FrontEnemy::TitleUpdate()
{
	if (alive) {
		object->SetPosition(position);
	}
	object->Update();
}

void FrontEnemy::appearance()
{
	position.z -= moveZ;
	if (position.z <= 50) {
		moveZ = 0;
		appFlag = false;
	}
}

void FrontEnemy::OnCollision()
{
	CreateParticle();
	life--;
	if (life <= 0) {
		alive = false;
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}