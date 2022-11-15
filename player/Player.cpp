#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("chr_sword"))
{
	//データ読み込み
	object->SetScale({ 2.5f, 2.5f, 1.5f });
}


void Player::Update()
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
		bullet->Update();
	}
	object->Update();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	position = object->GetPosition();
	//XMFLOAT3 rotation = playerObj->GetRotation();
	// 移動後の座標を計算
	if (input->PushKey(DIK_D)) { position.x += speed; }
	if (input->PushKey(DIK_A)) { position.x -= speed; }
	if (input->PushKey(DIK_W)) { position.z += speed; }
	if (input->PushKey(DIK_S)) { position.z -= speed; }

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
		position.y += g;
	}

	//ジャンプ
	if (input->TriggerKey(DIK_SPACE) && !jumpFlag) {
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

void Player::Shoot() 
{
	//コンストラクタ呼ぶよ
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		//初期化行くよ
		newBullet->Initialize(position);
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
	position.z -= speed;
	position.y += speed2;
	//speed2 -= t;
	if (position.y >= -1 && position.y <= 0 && position.x <= 25 && position.x >= -25
		&& position.z <= 25 && position.z >= -25) {
		speed2 -= t;
	}
	object->SetPosition(position);
}

//void Player::FloorCollision() {
//	jumpFlag = false;
//}