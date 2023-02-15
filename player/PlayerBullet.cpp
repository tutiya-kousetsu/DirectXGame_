#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

//PlayerBullet::PlayerBullet() :PlayerBullet(Model::CreateFromOBJ("sphere"))
//{
//	//データ読み込み
//	object->SetScale({ 1.0f, 1.0f, 1.0f });
//
//}

PlayerBullet* PlayerBullet::Create(Model* model)
{
	//3Dオブジェクトのインスタンスを生成
	PlayerBullet* instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//モデルのセット
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
}

bool PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel)
{
	//プレイヤーの座標渡すよ
	position = pos;
	velocity = vel;
	return true;
}

void PlayerBullet::Update()
{
	if (disappearTime <= ++frameNum) {
		alive = false;
	}

	if (alive) {
		
		position.x += velocity.m128_f32[0];
		position.y += velocity.m128_f32[1];
		position.z += velocity.m128_f32[2];
	}
	// 座標の変更を反映
	Object3d::SetPosition(position);
	//ゲームオブジェクト更新
	Object3d::Update();
}

void PlayerBullet::Draw()
{
	if (alive) {
		Object3d::Draw();
	}
}

void PlayerBullet::OnCollision()
{
	alive = false;
}