#include "FrontEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Player.h"
#include "ParticleManager.h"

FrontEnemy::FrontEnemy() :FrontEnemy(Model::CreateFromOBJ("squid"))
{
	particleMan = ParticleManager::GetInstance();
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	object->SetRotation({ 0, 180, 0 });
	audio = Audio::GetInstance();
	audio->SoundLoadWave("enHit.wav");
}

FrontEnemy::~FrontEnemy()
{
}

bool FrontEnemy::Initialize(XMFLOAT3 position)
{
	this->position = position;
	AccessPhase();
	return true;
}

void FrontEnemy::Update()
{
	//弾のフラグがfalseになったら削除する
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
	});
	if (alive ) {
		appearance();
		if (!appFlag) {
			Shoot();
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

void FrontEnemy::FrontShoot()
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

		object->SetRotation({ rotx, roty, 0 });
	}

	//コンストラクタ呼ぶよ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化
	newBullet->Initialize(position, velocity);

	bullets.push_back(std::move(newBullet));
}

void FrontEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		FrontShoot();
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void FrontEnemy::OnCollision()
{
	audio = Audio::GetInstance();
	audio->SoundPlayWave("enHit.wav", false);
	for (int j = 0; j < 100; j++) {
		//for (int i = 0; i < 11; i++) {
		DirectX::XMFLOAT3 pos = object->GetPosition();

		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//追加
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
		//}
	}

	life--;
	if (life == 0) {
			alive = false;
	}
}

void FrontEnemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	XMVECTOR worldPos{};

	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;


	return worldPos;
}

void FrontEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}