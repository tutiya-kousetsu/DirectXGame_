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
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//奥に進むよ
	//if (input->TriggerKey(DIK_SPACE)) {
		//apower++;
		position.z++;
	//}
	/*if (power == 1) {
		position.z++;
	}
	else if (power == 2) {
		position.z++;
	}
	else{
		position.z++;
	}*/

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

void PlayerBullet::OnCollision()
{
	alive = false;
}