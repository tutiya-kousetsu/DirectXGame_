#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(enemyObj);
	delete(frameObj);
	delete(enemyModel);
	delete(frameModel);
}

void Enemy::Initialize()
{
	//敵オブジェット
	enemyModel = Model::LoadFromObj("BlueBox");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(enemyModel);
	enemyObj->SetScale({ 0.65f, 0.65f, 0.65f });

	//枠のオブジェット
	frameModel = Model::LoadFromObj("ClearBox");
	frameObj = Object3d::Create();
	frameObj->SetModel(frameModel);
	frameObj->SetScale({ 0.66f, 0.66f, 0.66f });

	// 現在の座標を取得
	position = enemyObj->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10～-10の範囲
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0の範囲
	int z = rand() % 400;
	float z2 = (float)z / 10 - 20;//6~0の範囲
	position = { x2, y2, z2 };
	// 座標の変更を反映
	enemyObj->SetPosition(position);
	frameObj->SetPosition(position);

}

void Enemy::Update()
{
	UpdateAliveFlag();

	enemyObj->Update();
	frameObj->Update();
}

void Enemy::UpdateAliveFlag()
{
	if (aliveFlag == 1) {
		if (frameFlag == 0) {
			frameTimer++;
		}

		if (frameTimer >= 180) {
			frameFlag = 1;
		}

		enemyTimer++;
		//八秒たったら
		if (enemyTimer >= 480) {
			aliveFlag = 0;
			frameFlag = 0;
			frameTimer = 0;
			flashingTimer = 0;
		}
	}

	if (aliveFlag == 0) {
		//乱数で位置指定
		int x = rand() % 200;
		float x2 = (float)x / 10 - 10;//10～-10の範囲
		int y = rand() % 60;
		float y2 = (float)y / 10;//6～0の範囲
		enemyObj->SetPosition({ x2, y2, 5 });
		frameObj->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		frameFlag = 0;
		frameTimer = 0;
		aliveFlag = 1;
	}
}

void Enemy::Draw()
{
	//フラグ0の時に枠だけ表示
	if (frameFlag == 0) {
		frameObj->Draw();
	}
	//フラグ1で敵表示
	if (frameFlag == 1) {
		if (aliveFlag == 1) {
			enemyObj->Draw();
		}
	}
}
