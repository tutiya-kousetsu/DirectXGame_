#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
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

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	Object3d::SetPosition({ 0,0,0 });
	//コライダーの追加
	float radius = 0.6f;
	//半径だけ足元から浮いた座標を球の中心にする
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	move();
	jump();
	playerRot();
	if (input->TriggerMouseLeft()) {
		Shoot();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}
	//particleMan->Update();
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
	//重力
	//position.y -= g;

	//マウスの右をクリックしたらジャンプ
	if (!onGround) {

		position.y += jumpSpeed;
		//ジャンプの速度を0.04ずつ下げていく
		jumpSpeed -= 0.05f;
	}
	//ジャンプフラグが1になったら
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		//ジャンプの高さ
		jumpSpeed = 1.0f;
	}
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 5.0f + adsDistance)) {
			onGround = true;
			//position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 4.0f);
			//position.y += g;
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
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 5.5f)) {
			// 着地
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 5.5f);
			// 行列の更新など
			Object3d::Update();
		}
	}
	//ポジションが0になったらジャンプフラグを切る
	/*if (position.y <= 0) {
		jumpFlag = false;
	}*/
	// 落下処理
	//if (input->TriggerMouseRight() && !onGround || input->TriggerKey(DIK_SPACE) && !onGround) {
	//	// 下向き加速度
	//	const float fallAcc = -0.01f;
	//	const float fallVYMin = -0.5f;
	//	// 加速
	//	fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	//	// 移動
	//	position.x += fallV.m128_f32[0];
	//	position.y += fallV.m128_f32[1];
	//	position.z += fallV.m128_f32[2];
	//}
	//// ジャンプ操作
	//else if (input->TriggerKey(DIK_SPACE)) {
	//	onGround = false;
	//	const float jumpVYFist = 1.2f;
	//	fallV = { 0, jumpVYFist, 0, 0 };
	//}
	// 座標の変更を反映
	///Object3d::SetPosition(position);
	

	// 接地状態
	//if (onGround) {
	//	// スムーズに坂を下る為の吸着距離
	//	const float adsDistance = 0.2f;
	//	// 接地を維持
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//		// 行列の更新など
	//		Object3d::Update();
	//	}
	//	// 地面がないので落下
	//	else {
	//		onGround = false;
	//		fallV = {};
	//	}
	//}
	//// 落下状態
	//else if (fallV.m128_f32[1] <= 0.0f) {
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
	//		// 着地
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//		// 行列の更新など
	//		Object3d::Update();
	//	}
	//}
}

void Player::playerRot()
{
	//Input* input = Input::GetInstance();
	//Input::MouseMove mouseMove = input->GetMouseMove();
	//float dy = mouseMove.lX * scaleY;
	//angleY = -dy * XM_PI;

	//if (angleY) { rotation.y += speed + 1; }
	//if (-angleY) { rotation.y -= speed + 1; }

	//Object3d::SetRotation(rotation);
}

void Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());
	//コンストラクタ呼ぶよ
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//初期化行くよ
	newBullet->Initialize(position, velocity);

	//弾を登録する
	bullets.push_back(std::move(newBullet));

}

void Player::OnCollision(const CollisionInfo& info)
{
	//for (int i = 0; i < 1; i++) {
	//	//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
	//	const float md_pos = 5.0f;
	//	DirectX::XMFLOAT3 pos = Object3d::GetPosition();
	//	//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
	//	const float md_vel = 0.1f;
	//	DirectX::XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
	//	DirectX::XMFLOAT3 acc{};
	//	const float rnd_acc = 0.005f;
	//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//	//追加
	//	particleMan->Add(60, DirectX::XMFLOAT3(info.inter.m128_f32), vel, acc, 0.0f, 1.0f);
 //	}
}

void Player::FloorCollision()
{
	//範囲選択
	if (position.x <= 33.f && position.x >= -33.f
		&& position.z <= 33.f && position.z >= -33.f
		) {
		position.y += g;
	}
	else {
		onGround = true;
	}
	Object3d::SetPosition(position);
}

void Player::Draw()
{
	if (alive) {
		Object3d::Draw();
		//particleMan->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
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
