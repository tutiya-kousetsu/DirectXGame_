#include "PlayerBullet.h"
#include <cassert>
#include "Input.h"

//PlayerBullet::PlayerBullet() :PlayerBullet(Model::CreateFromOBJ("sphere"))
//{
//	//�f�[�^�ǂݍ���
//	object->SetScale({ 1.0f, 1.0f, 1.0f });
//
//}

PlayerBullet* PlayerBullet::Create(Model* model)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PlayerBullet* instance = new PlayerBullet();
	if (instance == nullptr) {
		return nullptr;
	}

	//���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

PlayerBullet::~PlayerBullet()
{
}

bool PlayerBullet::Initialize(DirectX::XMFLOAT3 pos, const XMVECTOR& vel)
{
	//�v���C���[�̍��W�n����
	position = pos;
	velocity = vel;
	return true;
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
	Object3d::SetPosition(position);
	//�Q�[���I�u�W�F�N�g�X�V
	Object3d::Update();
}

void PlayerBullet::Draw()
{
	if (alive) {
		Object3d::Draw();
	}
}

void PlayerBullet::OnCollision()
{
	alive = false;
}