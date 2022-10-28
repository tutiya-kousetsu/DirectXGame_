#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	move();
	jump();
	Shoot();
	for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {
		bullet->Update();
	}
	//std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

	//�}�E�X�̍��N���b�N���������Ƃ�
	if (input->PushMouseLeft()) {
	}

	object->Update();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	XMFLOAT3 position = object->GetPosition();
	//XMFLOAT3 rotation = playerObj->GetRotation();
	// �ړ���̍��W���v�Z
	if (input->PushKey(DIK_D)) { position.x += speed; }
	if (input->PushKey(DIK_A)) { position.x -= speed; }
	if (input->PushKey(DIK_W)) { position.z += speed; }
	if (input->PushKey(DIK_S)) { position.z -= speed; }

	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = object->GetPosition();
	//�d��
	position.y -= g;
	if (position.y <= 0) position.y += g;

	//�W�����v
	if (input->TriggerKey(DIK_SPACE) && !jumpFlag) {
		jumpFlag = true;
		//�W�����v�̍���
		jumpSpeed = 1.0f;
	}
	//�W�����v�t���O��1�ɂȂ�����
	if (jumpFlag) {
		position.y += jumpSpeed;
		//�W�����v�̑��x��0.04�������Ă���
		jumpSpeed -= 0.04f;
	}

	//�|�W�V������0�ɂȂ�����W�����v�t���O��؂�
	if (position.y <= 0) {
		jumpFlag = false;
	}
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
}

void Player::Shoot() 
{
	Input* input = Input::GetInstance();
	
	if (input->TriggerKey(DIK_SPACE)) {
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {

			bulPos = position;
			bullet->SetPosition(bulPos);
		}
		//bulPos = bullet->GetPosition();
		//bulPos.z += 0.2f;

		//�e��o�^����
		bullet.push_back(std::move(newBullet));
	}
	
}

void Player::Draw()
{
	if (alive) {
		object->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : this->bullet) {
		bullet->Draw();
	}
	
}
