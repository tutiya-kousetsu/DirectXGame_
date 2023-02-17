#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

PlayerBullet::PlayerBullet() :PlayerBullet(Model::CreateFromOBJ("sphere"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });

}

//PlayerBullet* PlayerBullet::Create(Model* model)
//{
//	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
//	PlayerBullet* instance = new PlayerBullet();
//	if (instance == nullptr) {
//		return nullptr;
//	}
//
//	//���f���̃Z�b�g
//	if (model) {
//		instance->SetModel(model);
//	}
//
//	return instance;
//}

PlayerBullet::~PlayerBullet()
{
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
		position.y += velocity.m128_f32[1];
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