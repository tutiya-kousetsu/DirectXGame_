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
}

void Enemy::Update()
{

	if (aliveFlag == 1) {

		if (frameFlag == 0) {
			frameTimer++;
		}

		if (frameTimer >= 240) {
			frameFlag = 1;
		}

		enemyTimer++;
		// 現在の座標を取得
		position = enemyObj->GetPosition();
		position.z = 5;
		// 座標の変更を反映
		enemyObj->SetPosition(position);
		frameObj->SetPosition(position);
		//画面は次まで行ったら
		if (enemyTimer >= 540) {
			aliveFlag = 0;
			frameFlag = 0;
			frameTimer = 0;
			flashingTimer = 0;
			//flashingFlag = 0;
		}
	}

	if (aliveFlag == 0) {
		//乱数で位置指定
		int x = rand() % 160;
		float x2 = (float)x / 10 - 8;//8～-8の範囲
		int y = rand() % 160;
		float y2 = (float)y / 10 - 8;//8～-8の範囲
		enemyObj->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		flashingTimer = 0;
		flashingFlag = 0;
		aliveFlag = 1;
	}
	enemyObj->Update();
	frameObj->Update();
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
