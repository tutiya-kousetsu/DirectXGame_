#include "EnemyBullet.h"
#include <cassert>
#include "Input.h"

EnemyBullet::EnemyBullet() :EnemyBullet(Model::LoadFromObj("sphere"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void EnemyBullet::Initialize(XMFLOAT3 pos)
{
	//�G�̍��W�n����
	position = pos;
}

void EnemyBullet::Update()
{
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//���ɐi�ނ�
	position.z--;
	//�G���������l���Q�[���I�u�W�F�N�g�ɓn����
	object->SetPosition(position);
	//�Q�[���I�u�W�F�N�g�X�V
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