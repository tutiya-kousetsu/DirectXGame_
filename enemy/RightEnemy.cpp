#include "RightEnemy.h"
#include "Player.h"

//RightEnemy::RightEnemy()
//{
//
//	SetScale({ 1.0f, 1.0f, 1.0f });
//
//	// 現在の座標を取得
//	position = GetPosition();
//	int x = rand() % 700;
//	float x2 = (float)x / 10 - 35;//10〜-10の範囲
//	int y = rand() % 70;
//	float y2 = (float)y / 10;//6~0の範囲
//	int z = rand() % 700;
//	float z2 = (float)z / 10 - 35;//6~0の範囲
//	position = { 35, 35, z2 };
//	// 座標の変更を反映
//	SetPosition(position);
//}

RightEnemy::~RightEnemy()
{
}

bool RightEnemy::Initialize()
{
	//enemy = enemy->Create(Model::CreateFromOBJ("BlueBox"));

	AccessPhase();
	return true;
}

void RightEnemy::Update()
{
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();

		if (!appFlag) {
			Shoot();

			position.z += move;
			if (position.z >= 35) {
				move = move * -1;
			}
			if (position.z <= -35) {
				move = move * -1;
			}
		}
		object->SetPosition(position);
	}

	object->Update();
}

void RightEnemy::appearance()
{
	//描画されたら、敵をランダムで決めた位置の高さまでおろす
	position.y -= moveY;
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	if (position.y <= y2) {
		moveY = 0;
		appFlag = false;
	}
}

void RightEnemy::RightShoot()
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
	//コンストラクタ呼ぶよ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化行くよ
	newBullet->Initialize(position, velocity);
	//弾を登録する
	bullets.push_back(std::move(newBullet));
}

void RightEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		RightShoot();
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void RightEnemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}


XMVECTOR RightEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void RightEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}