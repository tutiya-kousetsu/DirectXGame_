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
	if (disappearTime < ++frameNum) {
		alive = false;
	}
	//���ɐi�ނ�
	//if (input->TriggerKey(DIK_SPACE)) {
		//apower++;
		position.z++;
	//}
	/*if (power == 1) {
		position.z++;
	}
	else if (power == 2) {
		position.z++;
	}
	else{
		position.z++;
	}*/

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

void PlayerBullet::OnCollision()
{
	alive = false;
}