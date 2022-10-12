#include "Player.h"
#include "Input.h"
#include "Shoot.h"
#include "DebugText.h"
#include "SphereCollider.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X����
	Player* instance = new Player();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize()) {
		delete instance;
		assert(0);
	}

	//���f���̃Z�b�g
	if (model) {
		instance->SetModel(model);
	}

	return instance;
}

Player::Player()
{
}

Player::~Player()
{
	delete(playerObj);
	delete(playerModel);
}

bool Player::Initialize()
{
	////�����o�ϐ��ɃR�s�[
	//this->input = input;


	if (!Object3d::Initialize()) {
		return false;
	}

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));

	return true;
}

void Player::Update(Shoot* shootPos)
{
	Input* input = Input::GetInstance();
	XMFLOAT3 playerPos;
	// �ړ���̍��W���v�Z
	//�ړ�
	/*if (input->PushKey(DIK_RIGHT)) { position.x += 0.15f; }
	if (input->PushKey(DIK_LEFT)) { position.x -= 0.15f; }
	if (input->PushKey(DIK_UP)) { position.y += 0.15f; }
	if (input->PushKey(DIK_DOWN)) { position.y -= 0.15f; }*/
	//��]
	if (input->PushKey(DIK_A)) { rotation.y -= 2.0f; }
	if (input->PushKey(DIK_D)) { rotation.y += 2.0f; }
	
	//�ړ��x�N�g����Y�����̊p�x�ŉ�]
	XMVECTOR move = { 0,0,0.1f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (input->PushKey(DIK_S)) {
		position.x -= move.m128_f32[0];
		position.z -= move.m128_f32[1];
		position.x -= move.m128_f32[2];
	}
	if (input->PushKey(DIK_W)) {
		position.x += move.m128_f32[0];
		position.z += move.m128_f32[1];
		position.x += move.m128_f32[2];
	}

	//�ړ��͈͂̐���
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// ���W�̕ύX�𔽉f
	playerObj->SetPosition(position);
	
	playerObj->Update();
	Object3d::Update();
}

void Player::OnCollision(const CollisionInfo& info)
{
	for (int i = 0; i < 4; i++) {

		enemy[i]->Hit();
	}
}

void Player::Draw()
{
	playerObj->Draw();
}
