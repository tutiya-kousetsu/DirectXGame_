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
	XMFLOAT3 rotation = playerObj->GetRotation();
	rotation.y += 90;
	
	// �ړ���̍��W���v�Z
	if (input->PushKey(DIK_D)) { position.x += 0.2f; }
	if (input->PushKey(DIK_A)) { position.x -= 0.2f; }
	if (input->PushKey(DIK_W)) { position.y += 0.2f; }
	if (input->PushKey(DIK_S)) { position.y -= 0.2f; }

	//�ړ��͈͂̐���
	if (position.x > 40) position.x = 40;
	if (position.x < -40) position.x = -40;

	// �W�����v
	const bool hitSpace = Input::GetInstance()->TriggerKey(DIK_SPACE);

	if (hitSpace && !jumpFlag) {
		jumpFlag = true;
		jumpSpead = 0.75f;

	}

	if (jumpFlag) {
		//auto position = playerObj->GetPosition();

		position.y += jumpSpead;

		jumpSpead -= 0.04f;
		if (position.y < jumpStartPos.y) {
			position.y = jumpStartPos.y;
			jumpFlag = false;
		}
		playerObj->SetPosition(position);
	}

	// ���W�̕ύX�𔽉f
	playerObj->SetPosition(position);
	playerObj->SetRotation(rotation);
	playerObj->Update();
}

void Player::Draw()
{
	playerObj->Draw();
}
