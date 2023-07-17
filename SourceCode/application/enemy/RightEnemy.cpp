#include "RightEnemy.h"
#include "Player.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "ParticleManager.h"

RightEnemy::RightEnemy() :RightEnemy(Model::CreateFromOBJ("purpleSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0,270,0 });
	audio = Audio::GetInstance();
	audio->SoundLoadWave("enHit.wav");
}

RightEnemy::~RightEnemy()
{
}

bool RightEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	AccessPhase();
	return true;
}

void RightEnemy::Update()
{
	//’e‚Ìƒtƒ‰ƒO‚ªfalse‚É‚È‚Á‚½‚çíœ‚·‚é
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();

		if (!appFlag) {
			Shoot(position);

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

XMVECTOR RightEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos‚Éplayer‚Ìposition‚ğ‚¢‚ê‚é
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}