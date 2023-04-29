#include "PlayerBullet.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include <cassert>

PlayerBullet::PlayerBullet() :PlayerBullet(Model::CreateFromOBJ("playerCube"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(XMFLOAT3 pos, const XMVECTOR& vel)
{
	//敵の座標渡すよ
	position = pos;
	velocity = vel;
}

void PlayerBullet::Update()
{
	disappearTime--;
	if (disappearTime < ++frameNum) {
		alive = false;
	}

	if (alive) {
		position.x += velocity.m128_f32[0];
		position.y += velocity.m128_f32[1];
		position.z += velocity.m128_f32[2];

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