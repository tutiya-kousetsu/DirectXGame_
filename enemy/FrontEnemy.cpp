#include "FrontEnemy.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "SphereCollider.h"
#include "Player.h"
#include "ParticleManager.h"

FrontEnemy::FrontEnemy() :FrontEnemy(Model::CreateFromOBJ("BlueBox"))
{
	particleMan = ParticleManager::GetInstance();
}

FrontEnemy::~FrontEnemy()
{
	//for (int i = 0; i < 11; i++) {
		//delete object;
	//}
}

bool FrontEnemy::Initialize(XMFLOAT3 position)
{
	//for (int i = 0; i < 11; i++) {
		/*model[i] = Model::CreateFromOBJ("BlueBox");
		object[i] = Object3d::Create();
		object[i]->SetModel(model[i]);*/
		object->SetScale({ 1.f, 1.f, 1.f });
		object->SetRotation({ 0, 180, 0 });
	//}
	AccessPhase();
	this->position = position;
	//position = object->GetPosition();
	//position = { 8, 8, 40 };
	//for (int i = 0; i < 11; i++) {
		//position = object->GetPosition();
	//}
	//position[0] = { 8, 8, 60 };
	//position[1] = { -8, 8, 60 };
	/*position[2] = { 8, 8, 60 };
	position[3] = { -8, 8, 60 };
	position[4] = { 8, 8, 60 };
	position[5] = { -8, 8, 60 };
	position[6] = { 8, 8, 60 };
	position[7] = { -8, 8, 60 };
	position[8] = { 8, 8, 60 };
	position[9] = { -8, 8, 60 };
	position[10] = { 8, 8, 60 };*/
	//for (int i = 0; i < 11; i++) {
		//object->SetPosition(position);
	//}
	return true;
}

void FrontEnemy::Update()
{
	//for (int i = 0; i < 11; i++) {
	if (alive) {
		//手前に移動してくる処理
		appearance();
		if (appFlag) {
			Shoot();
		}
		object->SetPosition(position);
		object->Update();
	}
	/*if (!alive[0]) {
		appearance();
		if (appFlag) {
			Shoot();
		}
		object[1]->Update();
	}*/
	//}
}

void FrontEnemy::appearance()
{
	for (int i = 0; i < 11; i++) {
		//手前に移動させる
		position.z -= moveZ;
		//int y = rand() % 70;
		//float y2 = (float)y / 10;//6~0の範囲
		if (position.z <= 50) {
			moveZ = 0;
			appFlag = true;
		}
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

	//コンストラクタ
	EnemyBullet* newBullet = new EnemyBullet();
	//初期化
	newBullet->Initialize(position, velocity);
	bullet.reset(newBullet);
}

void FrontEnemy::Shoot()
{

	shootTimer--;
	if (shootTimer < 0) {
		FrontShoot();
		shootTimer = kShootInterval;
	}
	//for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
	if (bullet) {
		bullet->Update();
	}
}

void FrontEnemy::OnCollision()
{
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
		for (int i = 0; i < 11; i++) {
			alive = false;
		}
	}
}

void FrontEnemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		//for (int i = 0; i < 11; i++) {
			object->Draw();
		//}
		//for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		if (bullet) {
			bullet->Draw();
		}
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	XMVECTOR worldPos{};
	//for (int i = 0; i < 11; i++) {

		//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;


	//}
	return worldPos;
}

void FrontEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}