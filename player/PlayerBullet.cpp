#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

//PlayerBullet::PlayerBullet() :PlayerBullet(Model::CreateFromOBJ("sphere"))
//{
//	//データ読み込み
//	object->SetScale({ 1.0f, 1.0f, 1.0f });
//}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel)
{
	object.reset(new PlayerBulletObj());
	object->Initialize(Model::CreateFromOBJ("sphere"));
	//敵の座標渡すよ
	position = pos;
	velocity = vel;
	//this->power = power;
}

void PlayerBullet::Update()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}

	if (alive) {
		position.x += velocity.m128_f32[0];
		position.y += velocity.m128_f32[1];
		position.z += velocity.m128_f32[2];

		//敵が動いた値をゲームオブジェクトに渡す
		object->SetPosition(position);
		//ゲームオブジェクト更新
		object->Update();
	}
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