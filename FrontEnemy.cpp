#include "FrontEnemy.h"

FrontEnemy::FrontEnemy()
{
	//enemy = new Enemy();

	SetScale({ 1.0f, 1.0f, 1.0f });

	// Œ»İ‚ÌÀ•W‚ğæ“¾
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10`-10‚Ì”ÍˆÍ
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0‚Ì”ÍˆÍ
	int z = rand() % 700;
	//float z2 = (float)z / 10 - 35;//6~0‚Ì”ÍˆÍ
	position = { x2, y2, 35 };
	// À•W‚Ì•ÏX‚ğ”½‰f
	SetPosition(position);
}

void FrontEnemy::Initialize()
{
	AccessPhase();
}

void FrontEnemy::Update()
{
	if (alive) {
		shootTimer--;
		if (shootTimer < 0) {
			Shoot();

			shootTimer = kShootInterval;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Update();
		}
		//position.x += 0.1f;
		//Enemy::object->SetPosition(position);
	}
	object->Update();
}

void FrontEnemy::Draw()
{
	if (alive) {
		object->Draw();
	}
}

void FrontEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}