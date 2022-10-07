#include "Shoot.h"

Shoot::Shoot()
{
}

Shoot::~Shoot()
{
	delete(shootObj);

	delete(shootModel);
}

void Shoot::Initialize(Input* input, Player* player)
{
	//メンバ変数にコピー
	this->input = input;
	this->player = player;
	//データ読み込み
	shootModel = Model::LoadFromObj("sphere");
	shootObj = Object3d::Create();
	shootObj->SetModel(shootModel);
	shootObj->SetScale({ 0.5f, 0.5f, 0.5f });
}

void Shoot::Update()
{
	Input* input = Input::GetInstance();
	if (aliveFlag == 1) {
		XMFLOAT3 position = shootObj->GetPosition();

		position.z += 0.5f;

		// 座標の変更を反映
		shootObj->SetPosition(position);

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
			shootObj->SetPosition({ position.x, position.y, position.z });

			aliveFlag = 1;
		}
	}
	shootObj->Update();
}

void Shoot::Draw()
{
	if (aliveFlag == 1) {
		shootObj->Draw();
	}
}
