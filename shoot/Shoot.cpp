#include "Shoot.h"

Shoot::Shoot()
{
}

Shoot::~Shoot()
{
	for (int i = 0; i < 5; i++) {
		delete(shootObj[i]);
	}

	delete(shootModel);
}

void Shoot::Initialize(Input* input, Player* player)
{
	//メンバ変数にコピー
	this->input = input;
	this->player = player;
	//データ読み込み
	for (int i = 0; i < 5; i++) {
		shootModel = Model::LoadFromObj("sphere");
		shootObj[i] = Object3d::Create();
		shootObj[i]->SetModel(shootModel);
		shootObj[i]->SetScale({ 0.5f, 0.5f, 0.5f });
	}
}

void Shoot::Update()
{
	Input* input = Input::GetInstance();
	for (int i = 0; i < 5; i++) {
		if (aliveFlag == 1) {
			XMFLOAT3 position = shootObj[i]->GetPosition();

			position.z += 0.5f;

			// 座標の変更を反映
			shootObj[i]->SetPosition(position);

			//画面は次まで行ったら
			if (position.z > 33) {
				aliveFlag = 0;
			}
		}
		if (aliveFlag == 0) {
			//スペース押したら
			if (input->PushKey(DIK_SPACE)) {

				XMFLOAT3 position = player->GetPosition();

				// 座標の変更を反映
				shootObj[i]->SetPosition({ position.x, position.y, position.z });

				aliveFlag = 1;
			}
		}
		shootObj[i]->Update();
	}
}

void Shoot::Draw()
{
	if (aliveFlag == 1) {
		for (int i = 0; i < 5; i++) {
			shootObj[i]->Draw();
		}
	}
}
