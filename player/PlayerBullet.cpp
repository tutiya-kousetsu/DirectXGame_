#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::LoadFromObj("sphere"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void PlayerBullet::Initialize(XMFLOAT3 pos)
{
	//�v���C���[�̍��W�n����
	position = pos;
}

void PlayerBullet::Update()
{
	Input* input = Input::GetInstance();
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//���ɐi�ނ�
	position.z++;

	//���������l���Q�[���I�u�W�F�N�g�ɓn����
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