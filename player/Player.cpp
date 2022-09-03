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
	Input* input = Input::GetInstance();
	// 現在の座標を取得
	XMFLOAT3 position = playerObj->GetPosition();
	//XMFLOAT3 rotation = playerObj->GetRotation();
	//position.z += 0.15f;
	
	// 移動後の座標を計算
	if (input->PushKey(DIK_RIGHT)) { position.x += 0.15f; }
	if (input->PushKey(DIK_LEFT)) { position.x -= 0.15f; }
	if (input->PushKey(DIK_UP)) { position.y += 0.15f; }
	if (input->PushKey(DIK_DOWN)) { position.y -= 0.15f; }

	//移動範囲の制限
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// 座標の変更を反映
	playerObj->SetPosition(position);
	
	playerObj->Update();
}

void Player::Draw()
{
	playerObj->Draw();
}
