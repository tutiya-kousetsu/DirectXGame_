#include "FrontEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Player.h"
#include "ParticleManager.h"

FrontEnemy::~FrontEnemy()
{
}

bool FrontEnemy::Initialize()
{
	object.reset(new EnemyObject());
	object->Initialize(Model::CreateFromOBJ("BlueBox"));

	SetScale({ 1.0f, 1.0f, 1.0f });
	object->SetRotation({ 0, 180, 0 });
	particleMan = ParticleManager::GetInstance();
	// 現在の座標を取得
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10～-10の範囲
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	int z = rand() % 700;
	//float z2 = (float)z / 10 - 35;//6~0の範囲
	position = { x2, 35, 35 };
	// 座標の変更を反映
	SetPosition(position);
	AccessPhase();

	return true;
}

void FrontEnemy::Update()
{
	if (alive) {
		appearance();

		if (!appFlag) {
			/*SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
			assert(sphereCollider);
			Ray ray;
			ray.start = sphereCollider->center;
			ray.start.m128_f32[1] += sphereCollider->GetRadius();
			ray.dir = { 0,-1,0,0 };
			RaycastHit raycastHit;
			if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_ALLIES, &raycastHit, sphereCollider->GetRadius())) {
				OnCollision();
			}*/
			//敵が止まったらフラグを立てて弾を撃ち始める
			Shoot();
		}
		object->SetPosition(position);
	}

	object->Update();
}

void FrontEnemy::appearance()
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
	}
	//コンストラクタ呼ぶよ
	EnemyBullet* newBullet = new EnemyBullet();
	//初期化
	newBullet->Initialize(position, velocity);

	bullets.reset(newBullet);

}

void FrontEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		FrontShoot();
		shootTimer = kShootInterval;
	}
	if (bullets) {
		bullets->Update();
	}
}

void FrontEnemy::OnCollision()
{
	for (int j = 0; j < 100; j++) {
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
		if (bullets) {
			bullets->Draw();
		}
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
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