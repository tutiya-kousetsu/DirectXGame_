#include "Enemy.h"
#include "Player.h"
Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = object->GetPosition().x;
	worldPos.m128_f32[1] = object->GetPosition().y;
	worldPos.m128_f32[2] = object->GetPosition().z;

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
	position = { x2, y2, 35 };

	// 座標の変更を反映
	object->SetPosition(position);
}

void Enemy::Update()
{
	if (alive) {
		XMFLOAT3 rotation = object->GetRotation();
		rotation = { 0, 180, 0 };

		shootTimer--;
		if (shootTimer < 0) {
			Shoot();

			shootTimer = kShootInterval;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
				bullet->Update();
		}
		object->SetRotation(rotation);
	}
	object->Update();
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
	assert(this->player);

	//弾の速度
	const float kBulletSpeed = 1.0f;
	XMVECTOR playerPos = player->GetWorldPosition();
	XMVECTOR enemyPos = GetWorldPosition();

	// 速度を計算
		// 自分から標的までのベクトル
	velocity = {
		playerPos.m128_f32[0] - enemyPos.m128_f32[0],
		playerPos.m128_f32[1] - enemyPos.m128_f32[1],
		playerPos.m128_f32[2] - enemyPos.m128_f32[2]
	};
	// XMVECTORに変換
	//XMVECTOR vectorVel = XMLoadFloat3(&velocity);
	// 大きさを1にする
	velocity = XMVector3Normalize(velocity);
	// 大きさを任意の値にする
	velocity = XMVectorScale(velocity, 0.8f);
	// FLOAT3に変換
	//XMStoreFloat3(&vel, vectorVel);

	// 標的に向ける
	float rotx = atan2f(velocity.m128_f32[1], velocity.m128_f32[2]);
	float roty = atan2f(velocity.m128_f32[0], velocity.m128_f32[2]);

	//コンストラクタ呼ぶよ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化行くよ
	newBullet->Initialize(position, velocity);
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

XMVECTOR Enemy::GetWorldPosition() {
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}