#include "Player.h"
#include "Input.h"
#include "Shoot.h"
#include "DebugText.h"
#include "collision/SphereCollider.h"
#include "Enemy.h"
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

	//�f�[�^�ǂݍ���
	playerModel = Model::LoadFromObj("PlayerRed");
	playerObj = Object3d::Create();
	playerObj->SetModel((playerModel));
	playerObj->SetScale({ 0.75f, 0.75f, 0.75f });

	//�f�[�^�ǂݍ���
	shootModel = Model::LoadFromObj("sphere");
	shootObj = Object3d::Create();
	shootObj->SetModel(shootModel);
	shootObj->SetScale({ 0.5f, 0.5f, 0.5f });

	if (!Object3d::Initialize()) {
		return false;
	}

	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR({ 0,radius,0,0 }), radius));

	return true;
}

//�ړ�����
void Player::Move() {
	Input* input = Input::GetInstance();
	//��]
	if (input->PushKey(DIK_A)) { rotation.y -= 2.0f; }
	if (input->PushKey(DIK_D)) { rotation.y += 2.0f; }

	//�ړ��x�N�g����Y�����̊p�x�ŉ�]
	XMVECTOR move = { 0,0,0.5f,0 };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	if (input->PushKey(DIK_S)) {
		position.x -= move.m128_f32[0];
		position.z -= move.m128_f32[1];
		position.x -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W)) {
		position.x += move.m128_f32[0];
		position.z += move.m128_f32[1];
		position.x += move.m128_f32[2];
	}

	//�ړ��͈͂̐���
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// ���W�̕ύX�𔽉f
	playerObj->SetPosition(position);
}

void Player::Update()
{
	Move();
	playerObj->Update();

}


void Player::OnCollision(const CollisionInfo& info)
{
	playerHp--;
}

void Player::Draw()
{
	playerObj->Draw();
}
