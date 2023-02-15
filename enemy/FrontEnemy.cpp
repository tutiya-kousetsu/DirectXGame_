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

FrontEnemy* FrontEnemy::Create(Model* model)
{
	// オブジェクトのインスタンスを生成
	FrontEnemy* instance = new FrontEnemy();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool FrontEnemy::Initialize(Model* model)
{
	if (!Object3d::Initialize())
	{
		return false;
	}

	SetModel(model);

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);

	//属性の追加(敵)
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);

	SetScale({ 1.0f, 1.0f, 1.0f });
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
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
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
		Object3d::SetPosition(position);
	}

	Object3d::Update();
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
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化行くよ
	newBullet->Initialize(position, velocity);
	//弾を登録する
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
		Object3d::Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
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