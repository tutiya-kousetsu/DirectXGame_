#include "Enemy.h"

Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.65f, 1.65f, 1.65f });
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	AccessPhase();

	// 現在の座標を取得
	position = object->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10〜-10の範囲
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	position = { x2, y2, 30 };

	// 座標の変更を反映
	object->SetPosition(position);
}

void Enemy::Updata()
{
	if (alive) {
		shootTimer--;
		if (shootTimer < 0) {
			Shoot();

			shootTimer = kShootInterval;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
				bullet->Updata();
		}
	}
	object->Updata();
}

void Enemy::UpdateAliveFlag()
{
	//if (aliveFlag == 1) {
	//	//enemyTimer++;
	//	//八秒たったら
	//	if (enemyTimer >= 300) {
	//		aliveFlag = 0;
	//	}
	//	
	//}

	//if (aliveFlag == 0) {
	//	//乱数で位置指定
	//	//int x = rand() % 200;
	//	//float x2 = (float)x / 10 - 10;//10〜-10の範囲
	//	//int y = rand() % 60;
	//	//float y2 = (float)y / 10;//6〜0の範囲
	//	//object->SetPosition({ x2, y2, 5 });
	//	//enemyTimer = 0;
	//	//aliveFlag = 1;
	//	//for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
	//	//	bullet->SetPos({ x2, y2, 5 });
	//	//}
	//}
}

void Enemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}

}

void Enemy::Shoot()
{
	//コンストラクタ呼ぶよ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化行くよ
	newBullet->Initialize(position);
	//弾を登録する
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision()
{
	alive = false;
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bulFlag = bullet->GetAlive();
		bulFlag = false;
	}
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}