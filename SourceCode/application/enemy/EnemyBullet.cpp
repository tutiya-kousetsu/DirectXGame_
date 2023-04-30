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
}

void EnemyBullet::Initialize(XMFLOAT3 pos, const XMVECTOR& vel)
{
	//�G�̍��W�n����
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
	
	//�G���������l���Q�[���I�u�W�F�N�g�ɓn��
	object->SetPosition(position);
	//�Q�[���I�u�W�F�N�g�X�V
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