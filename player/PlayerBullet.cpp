#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });

}

void PlayerBullet::Initialize(XMFLOAT3 pos, const XMVECTOR& vel)
{
	//プレイヤーの座標渡すよ
	position = pos;
	velocity = vel;
}

void PlayerBullet::Update()
{
	if (disappearTime <= ++frameNum) {
		alive = false;
	}

	if (alive) {
		
		position.x += velocity.m128_f32[0];
		position.z += velocity.m128_f32[2];
	}
	// 座標の変更を反映
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