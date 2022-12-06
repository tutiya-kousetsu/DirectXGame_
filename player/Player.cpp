#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	object->SetPosition({ 0, 0.0f, 0 });
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
	object->Update();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = object->GetPosition();
	float moveSpeed = 0.3f;
	// 前方向と横方向の単位ベクトルを作る
	XMVECTOR forwardVec = XMVectorSet(0, 0, 1, 1);
	XMVECTOR horizontalVec = XMVectorSet(1, 0, 0, 1);
	// プレイヤーの回転に合わせて回転させる(前後)
	forwardVec = XMVector3Rotate(forwardVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(object->GetRotation().x),
		XMConvertToRadians(object->GetRotation().y),
		XMConvertToRadians(object->GetRotation().z)));
	// プレイヤーの回転に合わせて回転させる(横)
	horizontalVec = XMVector3Rotate(horizontalVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(object->GetRotation().x),
		XMConvertToRadians(object->GetRotation().y),
		XMConvertToRadians(object->GetRotation().z)));
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
	object->SetPosition(position);

}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = object->GetPosition();
	//重力
	position.y -= g;
	//床の範囲 
	if (position.y >= -1  && position.y <= 0 && position.x <= 25 && position.x >= -25
		&& position.z <= 25 && position.z >= -25) {
		//position.y += g;
	}

	//マウスの右をクリックしたらジャンプ
	if (input->TriggerMouseRight() && !jumpFlag || input->TriggerKey(DIK_SPACE) && !jumpFlag) {
		jumpFlag = true;
		//ジャンプの高さ
		jumpSpeed = 1.0f;
	}
	//ジャンプフラグが1になったら
	if (jumpFlag) {
		position.y += jumpSpeed;
		//ジャンプの速度を0.04ずつ下げていく
		jumpSpeed -= 0.04f;
	}

	//ポジションが0になったらジャンプフラグを切る
	if (position.y <= 0) {
		jumpFlag = false;
	}
	// 座標の変更を反映
	object->SetPosition(position);
}

void Player::playerRot()
{
	//Input* input = Input::GetInstance();
	//Input::MouseMove mouseMove = input->GetMouseMove();
	//float dy = mouseMove.lX * scaleY;
	//angleY = -dy * XM_PI;

	//if (angleY) { rotation.y += speed + 1; }
	//if (-angleY) { rotation.y -= speed + 1; }

	//object->SetRotation(rotation);
}

void Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);
	
	velocity = XMVector3TransformNormal(velocity, object->GetMatWorld());
	//コンストラクタ呼ぶよ
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//初期化行くよ
	newBullet->Initialize(position, velocity);

	//弾を登録する
	bullets.push_back(std::move(newBullet));

}

void Player::OnCollision()
{
	position.y += g;
	object->SetPosition(position);
}

void Player::Draw()
{
	if (alive) {
		object->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
	
}

XMVECTOR Player::GetWorldPosition() 
{
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}
