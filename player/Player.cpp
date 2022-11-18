#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.35f, 1.35f, 1.35f });
	object->SetPosition({ 0, 5.0f, 0 });
}


void Player::Updata()
{
	Input* input = Input::GetInstance();

	move();
	jump();

	if (input->TriggerMouseLeft()) {
		Shoot();
		/*bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
			return !bullet->GetAlive();
			});*/
	}

	

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Updata();
	}
	object->Updata();
}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = object->GetPosition();
	//XMFLOAT3 rotation = playerObj->GetRotation();
	// �ړ���̍��W���v�Z
	if (input->PushKey(DIK_D)) { position.x += speed; }
	if (input->PushKey(DIK_A)) { position.x -= speed; }
	if (input->PushKey(DIK_W)) { position.z += speed; }
	if (input->PushKey(DIK_S)) { position.z -= speed; }
	if (input->PushKey(DIK_RIGHT)) { rotation.y += speed +1; }
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
	object->SetRotation(rotation);
}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = object->GetPosition();
	//�d��
	position.y -= g;
	//���͈̔� 
	if (position.y >= -1  && position.y <= 0 && position.x <= 25 && position.x >= -25
		&& position.z <= 25 && position.z >= -25) {
		position.y += g;
	}

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
	//�R���X�g���N�^�ĂԂ�
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		//�������s����
		newBullet->Initialize(position);
		
		//�e��o�^����
		bullets.push_back(std::move(newBullet));

}

void Player::Draw()
{
	if (alive) {
		object->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}
	
}


void Player::OnCollision()
{
	position.z -= speed;
	position.y += speed2;
	//speed2 -= t;
	if (position.y >= -1 && position.y <= 0 ) {
		speed2 -= t;
	}
	object->SetPosition(position);


}
