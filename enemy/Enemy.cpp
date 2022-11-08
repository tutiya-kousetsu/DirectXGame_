#include "Enemy.h"

Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 0.65f, 0.65f, 0.65f });
}


void Enemy::Initialize()
{
	//Shoot();

	
	AccessPhase();

	// 現在の座標を取得
	position = object->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10～-10の範囲
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	int z = rand() % 400;
	float z2 = (float)z / 10 - 20;//6~0の範囲
	position = { x2, y2, z2 };
	// 座標の変更を反映
	object->SetPosition(position);
	//frameObj->SetPosition(position);
	}

void Enemy::Update()
{

	shootTimer--;

	if (shootTimer < 0) {
		Shoot();

		shootTimer = kShootInterval;
	}

	UpdateAliveFlag();
	if (bullet) {
		bullet->Update();
	}
	object->Update();
}

void Enemy::UpdateAliveFlag()
{
	if (aliveFlag == 1) {
		enemyTimer++;
		//八秒たったら
		if (enemyTimer >= 480) {
			aliveFlag = 0;
		}
	}

	if (aliveFlag == 0) {
		//乱数で位置指定
		int x = rand() % 200;
		float x2 = (float)x / 10 - 10;//10～-10の範囲
		int y = rand() % 60;
		float y2 = (float)y / 10;//6～0の範囲
		object->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		aliveFlag = 1;
	}
}

void Enemy::Draw()
{
	//フラグ1で敵表示
		if (aliveFlag == 1) {
			object->Draw();
			
		}
		if (bullet) {
			bullet->Draw();
		}
}

void Enemy::Shoot()
{
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(position);
	
	bullet.reset(newBullet);

	//コンストラクタ呼ぶよ
	//bullet = new EnemyBullet();
	////初期化行くよ
	//bullet->Initialize(position);

}

void Enemy::OnCollision()
{
	aliveFlag = 0;
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}