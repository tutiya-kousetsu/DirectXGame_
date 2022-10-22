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
	move();
	playerObj->Update();
}

void Player::move()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	XMFLOAT3 position = playerObj->GetPosition();
	XMFLOAT3 rotation = playerObj->GetRotation();
	rotation.y += 90;
	position.y -= g;
	// �ړ���̍��W���v�Z
	if (input->PushKey(DIK_D)) { position.x += 0.2f; }
	if (input->PushKey(DIK_A)) { position.x -= 0.2f; }
	//if (input->PushKey(DIK_W)) { position.y += 0.2f; }
	//if (input->PushKey(DIK_S)) { position.y -= 0.2f; }

	//�ړ��͈͂̐���
	if (position.x > 10) position.x = 10;
	if (position.x < -10) position.x = -10;
	if (position.y <= 0) position.y += g;

	//�W�����v
	{
		if (input->TriggerKey(DIK_SPACE) && !jumpFlag) {
			jumpFlag = true;
			
			//�W�����v�̍���
			jumpSpeed = 0.95;
		}
		//�W�����v�t���O��1�ɂȂ�����
		if (jumpFlag) {
			position.y += jumpSpeed;
			secondFlag = true;
			//�W�����v�̑��x��0.04�������Ă���
			jumpSpeed -= 0.04;
		}
		if (input->TriggerKey(DIK_SPACE) && jumpFlag && secondFlag) {
			position.y += jumpSpeed;
			//�W�����v�̑��x��0.04�������Ă���
			jumpSpeed -= 0.04;
		}
		//�|�W�V������0�ɂȂ�����W�����v�t���O��؂�
		if (position.y <= 0) {
			jumpFlag = false;
			secondFlag = false;
		}
	}

	// ���W�̕ύX�𔽉f
	playerObj->SetPosition(position);
	playerObj->SetRotation(rotation);
}

void Player::Draw()
{
	playerObj->Draw();
}
