#include "LeftEnemy.h"
#include "Player.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "ParticleManager.h"

LeftEnemy::LeftEnemy() :LeftEnemy(Model::CreateFromOBJ("greenSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	audio = Audio::GetInstance();
	audio->SoundLoadWave("enHit.wav");
}

LeftEnemy::~LeftEnemy()
{
}

bool LeftEnemy::Initialize(XMFLOAT3 position, XMFLOAT3 rotation)
{
	this->position = position;
	this->rotation = rotation;
	SetRotation(rotation);
	AccessPhase();
	return true;
}

void LeftEnemy::Update()
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

void LeftEnemy::appearance()
{
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
	//worldPos‚Éplayer‚Ìposition‚ğ‚¢‚ê‚é
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}