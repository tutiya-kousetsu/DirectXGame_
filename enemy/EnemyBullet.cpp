#include "EnemyBullet.h"
#include <cassert>
#include "Input.h"

EnemyBullet::EnemyBullet() :EnemyBullet(Model::LoadFromObj("sphere"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void EnemyBullet::Initialize(XMFLOAT3 pos)
{
	//敵の座標渡すよ
	position = pos;
}

void EnemyBullet::Update()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//奥に進むよ
	position.z--;
	//敵が動いた値をゲームオブジェクトに渡すよ
	object->SetPosition(position);
	//ゲームオブジェクト更新
	object->Update();
}

void EnemyBullet::Draw()
{
	if (alive) {
		object->Draw();
	}
}

void EnemyBullet::OnCollision()
{
	alive = false;
}