#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });

}

void PlayerBullet::Initialize(const XMVECTOR& pos, const XMVECTOR& velocity)
{
	//プレイヤーの座標渡すよ
	position = pos;
	this->velocity = velocity;
}

void PlayerBullet::Updata()
{
	/*if (disappearTime <= ++frameNum) {
		alive = false;
	}*/
	//奥に進むよ
	//if (input->TriggerKey(DIK_SPACE)) {
	//	//apower++;
	//if (alive) {

	//	position.z++;
	//}

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
	object->Updata();
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