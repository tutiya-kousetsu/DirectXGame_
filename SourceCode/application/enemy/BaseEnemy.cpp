#include "BaseEnemy.h"
#include "ParticleManager.h"
#include "Player.h"

using namespace DirectX;

void BaseEnemy::AccessPhase(int32_t kShootInterval)
{
	shootTimer = kShootInterval;
}

BaseEnemy::BaseEnemy(Model* model, const DirectX::XMFLOAT3& position) :object(std::move(Object3d::Create()))
{
	object->SetModel(model);
	object->SetPosition(position);
	particleMan = ParticleManager::GetInstance();
	audio = Audio::GetInstance();
	audio->SoundLoadWave("enHit.wav");
}

void BaseEnemy::Update()
{
	object->Update();
}

void BaseEnemy::Shoot(XMFLOAT3 position, int32_t kShootInterval)
{
	shootTimer--;
	if (shootTimer < 0) {
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

		}

		//コンストラクタ呼ぶよ
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		//初期化
		newBullet->Initialize(position, velocity);

		bullets.push_back(std::move(newBullet));
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
	//弾のフラグがfalseになったら削除する
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});

}

void BaseEnemy::CreateParticle()
{
	audio = Audio::GetInstance();
	audio->SoundPlayWave("enHit.wav", false);
	for (int j = 0; j < 100; j++) {
		//for (int i = 0; i < 11; i++) {
		XMFLOAT3 pos = object->GetPosition();

		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.20f;
		XMFLOAT3 vel{};
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
}

void BaseEnemy::Draw()
{
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}

XMVECTOR BaseEnemy::GetWorldPosition()
{
	XMVECTOR worldPos{};

	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}
