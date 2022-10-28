#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	move();
	jump();
	Shoot();
	for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {
		bullet->Update();
	}
	//std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

	//マウスの左クリックを押したとき
	if (input->PushMouseLeft()) {
	}

	object->Update();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	XMFLOAT3 position = object->GetPosition();
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
	if (position.y <= 0) position.y += g;

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
	Input* input = Input::GetInstance();
	
	if (input->TriggerKey(DIK_SPACE)) {
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {

			bulPos = position;
			bullet->SetPosition(bulPos);
		}
		//bulPos = bullet->GetPosition();
		//bulPos.z += 0.2f;

		//弾を登録する
		bullet.push_back(std::move(newBullet));
	}
	
}

void Player::Draw()
{
	if (alive) {
		object->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {
		bullet->Draw();
	}
	
}
