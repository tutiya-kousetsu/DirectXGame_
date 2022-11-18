#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//データ読み込み
	object->SetScale({ 1.35f, 1.35f, 1.35f });
	object->SetPosition({ 0, 5.0f, 0 });
}


void Player::Updata()
{
	Input* input = Input::GetInstance();

	move();
	jump();

	if (input->TriggerMouseLeft()) {
		Shoot();
		/*bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
			return !bullet->GetAlive();
			});*/
	}

	

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Updata();
	}
	object->Updata();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = object->GetPosition();
	//XMVECTOR rotation = playerObj->GetRotation();
	// 移動後の座標を計算
	if (input->PushKey(DIK_D)) { position.m128_f32[0] += speed; }
	if (input->PushKey(DIK_A)) { position.m128_f32[0] -= speed; }
	if (input->PushKey(DIK_W)) { position.m128_f32[2] += speed; }
	if (input->PushKey(DIK_S)) { position.m128_f32[2] -= speed; }
	if (input->PushKey(DIK_RIGHT)) { rotation.y += speed +1; }
	if(input->PushKey(DIK_LEFT)) { rotation.y -= speed + 1; }
	// 座標の変更を反映
	object->SetPosition(position);
	object->SetRotation(rotation);
}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = object->GetPosition();
	//重力
	position.m128_f32[1] -= g;
	//床の範囲 
	if (position.m128_f32[1] >= -1  && position.m128_f32[1] <= 0 &&
		position.m128_f32[0] <= 25 && position.m128_f32[0] >= -25 &&
		position.m128_f32[2] <= 25 && position.m128_f32[2] >= -25) {
		position.m128_f32[1] += g;
	}

	//ジャンプ
	if (input->TriggerKey(DIK_SPACE) && !jumpFlag) {
		jumpFlag = true;
		//ジャンプの高さ
		jumpSpeed = 1.0f;
	}
	//ジャンプフラグが1になったら
	if (jumpFlag) {
		position.m128_f32[1] += jumpSpeed;
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

void Player::Shoot() 
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity(0, 0, kBulletSpeed);

	//コンストラクタ呼ぶよ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		//初期化行くよ
		newBullet->Initialize(position, velocity);
		
		//弾を登録する
		bullets.push_back(std::move(newBullet));

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


void Player::OnCollision()
{
	position.m128_f32[2] -= speed;
	position.m128_f32[1] += speed2;
	//speed2 -= t;
	if (position.m128_f32[1] >= -1 && position.m128_f32[1] <= 0 ) {
		speed2 -= t;
	}
	object->SetPosition(position);


}
