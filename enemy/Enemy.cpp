#include "Enemy.h"
#include "Player.h"
Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	//line = new Line();
	frontEne = new frontEnemy();
	leftEne = new leftEnemy();
	rightEne = new rightEnemy();
	backEne = new backEnemy();

	AccessPhase();
	for (int i = 0; i < 4; i++) {
		frontEne->frontPos[i] = object->GetPosition();

		// 現在の座標を取得
		//position = object->GetPosition();
		int x = rand() % 700;
		float x2 = (float)x / 10 - 35;//10～-10の範囲
		int y = rand() % 70;
		float y2 = (float)y / 10;//6~0の範囲
		int z = rand() % 700;
		//float z2 = (float)z / 10 - 35;//6~0の範囲
		frontEne->frontPos[i] = { x2, y2, 35 };

		// 座標の変更を反映
		object->SetPosition(frontEne->frontPos[i]);
	}
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
	//playerに向かって弾発射
	{
		assert(this->player);

		XMVECTOR playerPos = player->GetWorldPosition();
		XMVECTOR enemyPos = GetWorldPosition();

		//速度を計算
		//自分から標的までのベクトル
		velocity = {
			playerPos.m128_f32[0] - enemyPos.m128_f32[0],
			playerPos.m128_f32[1] - enemyPos.m128_f32[1],
			playerPos.m128_f32[2] - enemyPos.m128_f32[2]
		};
		//大きさを1にする(ベクトルを正規化して返してあげる関数)
		velocity = XMVector3Normalize(velocity);
		//大きさを任意の値にする
		velocity = XMVectorScale(velocity, 1.5f);

		//標的に向ける
		float rotx = atan2f(velocity.m128_f32[1], velocity.m128_f32[2]);
		float roty = atan2f(velocity.m128_f32[0], velocity.m128_f32[2]);
	}
	for (int i = 0; i < 4; i++) {

		//コンストラクタ呼ぶよ
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//初期化行くよ
		newBullet->Initialize(position, velocity);
		//弾を登録する
		bullets.push_back(std::move(newBullet));
	}
}

void Enemy::OnCollision()
{
	alive = false;
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}

XMVECTOR Enemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}