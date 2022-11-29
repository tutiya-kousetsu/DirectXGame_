#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });

}

void PlayerBullet::Initialize(XMFLOAT3 pos, const XMVECTOR& vel)
{
	//�v���C���[�̍��W�n����
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
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
	//�Q�[���I�u�W�F�N�g�X�V
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