#include "LeftEnemy.h"
#include "Player.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"

LeftEnemy* LeftEnemy::Create(Model* model)
{
	// オブジェクトのインスタンスを生成
	LeftEnemy* instance = new LeftEnemy();
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

LeftEnemy::~LeftEnemy()
{
}

bool LeftEnemy::Initialize(Model* model)
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
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);	SetScale({ 1.0f, 1.0f, 1.0f });

	SetScale({ 1.0f, 1.0f, 1.0f });
	// 現在の座標を取得
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10～-10の範囲
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	int z = rand() % 600;
	float z2 = (float)z / 10 - 30;//30~-30の範囲
	position = { -35, 35, z2 };
	// 座標の変更を反映
	SetPosition(position);
	AccessPhase();
	return true;
}

void LeftEnemy::Update()
{
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();
		if (!appFlag) {
			Shoot();

			//横移動
			position.z += move;
			if (position.z >= 30) {
				move = move * -1;
			}
			if (position.z <= -30) {
				move = move * -1;
			}
		}
		Object3d::SetPosition(position);
	}

	Object3d::Update();
}

void LeftEnemy::appearance()
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

void LeftEnemy::LeftShoot()
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
	//コンストラクタ呼ぶ
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//初期化
	newBullet->Initialize(position, velocity);
	//弾を登録する
	bullets.push_back(std::move(newBullet));
}

void LeftEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		LeftShoot();
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void LeftEnemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		Object3d::Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}


XMVECTOR LeftEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPosにplayerのpositionをいれる
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void LeftEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}