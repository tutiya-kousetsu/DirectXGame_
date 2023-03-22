#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
Player* Player::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}
	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

Player::~Player()
{
}

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	particleMan = ParticleManager::GetInstance();
	Object3d::SetPosition({ 0,0,0 });
	Object3d::SetScale({ 0.9f,0.9f,0.9f });
	//コライダーの追加
	float radius = 2.0f;
	//半径だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	//弾のフラグがfalseになったら削除する
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
	});
	move();
	jump();
	Input* input = Input::GetInstance();
		if (input->TriggerMouseLeft()) {
			Shoot();
		}
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Update();
	}
	Object3d::Update();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	DirectX::XMFLOAT3 position = Object3d::GetPosition();
	float moveSpeed = 0.3f;
	// 前方向と横方向の単位ベクトルを作る
	XMVECTOR forwardVec = XMVectorSet(0, 0, 1, 1);
	XMVECTOR horizontalVec = XMVectorSet(1, 0, 0, 1);
	// プレイヤーの回転に合わせて回転させる(前後)
	forwardVec = XMVector3Rotate(forwardVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// プレイヤーの回転に合わせて回転させる(横)
	horizontalVec = XMVector3Rotate(horizontalVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// 大きさをmoveSpeedにする
	forwardVec = XMVectorScale(forwardVec, moveSpeed);
	horizontalVec = XMVectorScale(horizontalVec, moveSpeed);

	if (input->PushKey(DIK_W)) {
		position.x += forwardVec.m128_f32[0];
		position.x += forwardVec.m128_f32[1];
		position.z += forwardVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_S)) {
		position.x -= forwardVec.m128_f32[0];
		position.x -= forwardVec.m128_f32[1];
		position.z -= forwardVec.m128_f32[2];
	}

	if (input->PushKey(DIK_D)) {
		position.x += horizontalVec.m128_f32[0];
		position.x += horizontalVec.m128_f32[1];
		position.z += horizontalVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_A)) {
		position.x -= horizontalVec.m128_f32[0];
		position.x -= horizontalVec.m128_f32[1];
		position.z -= horizontalVec.m128_f32[2];
	}
	// 座標の変更を反映
	Object3d::SetPosition(position);

}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = Object3d::GetPosition();

	//落下処理
	if (!onGround) {
		//下向きの加速度
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//加速
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//移動
		if (position.y >= -5.0f) {
			position.x += fallV.m128_f32[0];
			position.y += fallV.m128_f32[1];
			position.z += fallV.m128_f32[2];
		}
	}
	//ジャンプ操作
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.56f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}

	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(0.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.5f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.5f);
			// 行列の更新など
			Object3d::Update();
		}
		// 地面がないので落下
		else {
			onGround = false;
			fallV = {};
		}
	}
	// 落下状態
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.5f)) {
			// 着地
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.5f);

		}
	}
	// ワールド行列更新
	UpdateWorldMatrix();
	collider->Update();

	// 行列の更新など
	Object3d::Update();
}

bool Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());

	//コンストラクタ呼ぶよ
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//初期化
	newBullet->Initialize(position, velocity);

	bullets.push_back(std::move(newBullet));
	return true;
}


void Player::OnCollision(const CollisionInfo& info)
{

}

void Player::CreateParticle()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = Object3d::GetPosition();
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
}


void Player::Draw()
{
	if (alive) {
		Object3d::Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Draw();
	}
}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos{};

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}
