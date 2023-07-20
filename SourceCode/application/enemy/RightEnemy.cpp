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
	//’e‚Ì”­ŽËŠÔŠu
	AccessPhase(kShootInterval);
	return true;
}

void RightEnemy::Update()
{
	if (alive) {
		//“G‚Ì“oêŠÖ”
		appearance();
		//“G‚Ì“oê‚ªI‚í‚Á‚½‚ç’e‚ð”­ŽË‚·‚é
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
	//worldPos‚Éplayer‚Ìposition‚ð‚¢‚ê‚é
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}