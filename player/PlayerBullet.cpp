#include "PlayerBullet.h"
#include "Input.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
#include <cassert>

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel)
{
	object.reset(new PlayerBulletObject());
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

	/*	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
		assert(sphereCollider);

		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0,0 };
		RaycastHit raycastHit;*/

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