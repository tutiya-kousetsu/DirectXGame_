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

void PlayerBullet::Updata()
{
	if (disappearTime <= ++frameNum) {
		alive = false;
	}

	if (alive) {
		position.z ++;
	}
	// 座標の変更を反映
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