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

void PlayerBullet::Updata()
{
	if (disappearTime <= ++frameNum) {
		alive = false;
	}

	if (alive) {
		position.z ++;
	}
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
	//�Q�[���I�u�W�F�N�g�X�V
	object->Updata();
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