#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
	delete(playerBulletObj);

	delete(playerBulletModel);
}

void PlayerBullet::Initialize(Input* input, Player* player)
{
	//メンバ変数にコピー
	this->input = input;
	this->player = player;
	//データ読み込み
	playerBulletModel = Model::LoadFromObj("sphere");
	playerBulletObj = Object3d::Create();
	playerBulletObj->SetModel(playerBulletModel);
	playerBulletObj->SetScale({ 0.5f, 0.5f, 0.5f });
}

void PlayerBullet::Update()
{
	Input* input = Input::GetInstance();
	if (aliveFlag == 1) {
		XMFLOAT3 position = playerBulletObj->GetPosition();

		position.z += 0.5f;

		// 座標の変更を反映
		playerBulletObj->SetPosition(position);

		//画面は次まで行ったら
		if (position.z > 33) {
			aliveFlag = 0;
		}
	}
	if (aliveFlag == 0) {
		//スペース押したら
		if (input->TriggerKey(DIK_SPACE)) {

			XMFLOAT3 position = player->GetPosition();

			// 座標の変更を反映
			playerBulletObj->SetPosition({ position.x, position.y, position.z });

			aliveFlag = 1;
		}
	}
	playerBulletObj->Update();
}

void PlayerBullet::Draw()
{
	if (aliveFlag == 1) {
		playerBulletObj->Draw();
	}
}
