#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete(playerObj);
	delete(playerModel);
}

void Player::Initialize(Input* input)
{
	//メンバ変数にコピー
	this->input = input;

	//データ読み込み
	playerModel = Model::LoadFromObj("PlayerRed");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerObj->SetScale({ 0.75f, 0.75f, 0.75f });
}

void Player::Update()
{
	move();
	playerObj->Update();
}

void Player::move()
{
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	XMFLOAT3 position = playerObj->GetPosition();
	XMFLOAT3 rotation = playerObj->GetRotation();
	rotation.y += 90;
	position.y -= g;
	// 移動後の座標を計算
	if (input->PushKey(DIK_D)) { position.x += 0.2f; }
	if (input->PushKey(DIK_A)) { position.x -= 0.2f; }
	//if (input->PushKey(DIK_W)) { position.y += 0.2f; }
	//if (input->PushKey(DIK_S)) { position.y -= 0.2f; }

	//移動範囲の制限
	if (position.x > 10) position.x = 10;
	if (position.x < -10) position.x = -10;
	if (position.y <= 0) position.y += g;

	//ジャンプ
	{
		if (input->TriggerKey(DIK_SPACE) && !jumpFlag) {
			jumpFlag = true;
			
			//ジャンプの高さ
			jumpSpeed = 0.95;
		}
		//ジャンプフラグが1になったら
		if (jumpFlag) {
			position.y += jumpSpeed;
			secondFlag = true;
			//ジャンプの速度を0.04ずつ下げていく
			jumpSpeed -= 0.04;
		}
		if (input->TriggerKey(DIK_SPACE) && jumpFlag && secondFlag) {
			position.y += jumpSpeed;
			//ジャンプの速度を0.04ずつ下げていく
			jumpSpeed -= 0.04;
		}
		//ポジションが0になったらジャンプフラグを切る
		if (position.y <= 0) {
			jumpFlag = false;
			secondFlag = false;
		}
	}

	// 座標の変更を反映
	playerObj->SetPosition(position);
	playerObj->SetRotation(rotation);
}

void Player::Draw()
{
	playerObj->Draw();
}
