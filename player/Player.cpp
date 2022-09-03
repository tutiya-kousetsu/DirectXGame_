#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	delete(playerObj);
	delete(playerModel);
}

void Player::Initialize(Input* input)
{
	//�����o�ϐ��ɃR�s�[
	this->input = input;

	//�f�[�^�ǂݍ���
	playerModel = Model::LoadFromObj("PlayerRed");
	playerObj = Object3d::Create();
	playerObj->SetModel(playerModel);
	playerObj->SetScale({ 0.75f, 0.75f, 0.75f });
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	XMFLOAT3 position = playerObj->GetPosition();
	//XMFLOAT3 rotation = playerObj->GetRotation();
	//position.z += 0.15f;
	
	// �ړ���̍��W���v�Z
	if (input->PushKey(DIK_RIGHT)) { position.x += 0.15f; }
	if (input->PushKey(DIK_LEFT)) { position.x -= 0.15f; }
	if (input->PushKey(DIK_UP)) { position.y += 0.15f; }
	if (input->PushKey(DIK_DOWN)) { position.y -= 0.15f; }

	//�ړ��͈͂̐���
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// ���W�̕ύX�𔽉f
	playerObj->SetPosition(position);
	
	playerObj->Update();
}

void Player::Draw()
{
	playerObj->Draw();
}
