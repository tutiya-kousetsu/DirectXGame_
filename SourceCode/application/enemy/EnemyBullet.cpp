#include "EnemyBullet.h"
#include "SphereCollider.h"
#include <cassert>
#include "Input.h"

EnemyBullet::EnemyBullet() :EnemyBullet(Model::CreateFromOBJ("sphere"))
{
	object->SetScale({ 1.f, 1.f, 1.f });
}


EnemyBullet::~EnemyBullet()
{
	object.reset();
}

void EnemyBullet::Initialize(XMFLOAT3 pos, const XMVECTOR& vel)
{
	//敵の座標渡すよ
	position = pos;
	velocity = vel;
}

void EnemyBullet::Update()
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