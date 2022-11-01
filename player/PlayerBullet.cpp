#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void PlayerBullet::Initialize(XMFLOAT3 pos)
{
	//プレイヤーの座標渡すよ
	position = pos;
}

void PlayerBullet::Update()
{
	Input* input = Input::GetInstance();
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//奥に進むよ
	position.z++;

	//動かした値をゲームオブジェクトに渡すよ
	object->SetPosition(position);
	//ゲームオブジェクト更新
	object->Update();
}

void PlayerBullet::Draw()
{
	if (alive) {
		object->Draw();
	}
}