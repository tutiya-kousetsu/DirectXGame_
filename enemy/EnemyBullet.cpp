#include "EnemyBullet.h"
#include <cassert>
#include "Input.h"

EnemyBullet::EnemyBullet() :EnemyBullet(Model::LoadFromObj("sphere"))
{
	//データ読み込み
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void EnemyBullet::Initialize(XMVECTOR pos)
{
	//敵の座標渡すよ
	position = pos;
}

void EnemyBullet::Updata()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}

	if (alive) {
		position.z--;
	
	//敵が動いた値をゲームオブジェクトに渡す
	object->SetPosition(position);
	//ゲームオブジェクト更新
	object->Updata();
	}
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