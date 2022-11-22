#include "Player.h"
#include "Input.h"

Player::Player() :Player(Model::LoadFromObj("PlayerRed"))
{
	//�f�[�^�ǂݍ���
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	object->SetPosition({ 0, 0.0f, 0 });
}


void Player::Updata()
{
	Input* input = Input::GetInstance();

	move();
	jump();

	if (input->TriggerMouseLeft()) {
		Shoot();
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
	float moveSpeed = 0.3f;
	// �O�����Ɖ������̒P�ʃx�N�g�������
	XMVECTOR forwardVec = XMVectorSet(0, 0, 1, 1);
	XMVECTOR horizontalVec = XMVectorSet(1, 0, 0, 1);
	// �v���C���[�̉�]�ɍ��킹�ĉ�]������(�O��)
	forwardVec = XMVector3Rotate(forwardVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(object->GetRotation().x),
		XMConvertToRadians(object->GetRotation().y),
		XMConvertToRadians(object->GetRotation().z)));
	// �v���C���[�̉�]�ɍ��킹�ĉ�]������(��)
	horizontalVec = XMVector3Rotate(horizontalVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(object->GetRotation().x),
		XMConvertToRadians(object->GetRotation().y),
		XMConvertToRadians(object->GetRotation().z)));
	// �傫����moveSpeed�ɂ���
	forwardVec = XMVectorScale(forwardVec, moveSpeed);
	horizontalVec = XMVectorScale(horizontalVec, moveSpeed);

	XMFLOAT3 forward;
	XMStoreFloat3(&forward, forwardVec);

	XMFLOAT3 horizontal;
	XMStoreFloat3(&horizontal, horizontalVec);

	if (input->PushKey(DIK_W)) {
		position.x += forwardVec.m128_f32[0];
		//position.y += forwardVec.m128_f32[1];
		position.z += forwardVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_S)) {
		position.x -= forwardVec.m128_f32[0];
		//position.y -= forwardVec.m128_f32[1];
		position.z -= forwardVec.m128_f32[2];
	}

	if (input->PushKey(DIK_D)) {
		position.x += horizontalVec.m128_f32[0];
		//position.y += horizontalVec.m128_f32[1];
		position.z += horizontalVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_A)) {
		position.x -= horizontalVec.m128_f32[0];
		//position.y -= horizontalVec.m128_f32[1];
		position.z -= horizontalVec.m128_f32[2];
	}
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);

	if (input->PushKey(DIK_RIGHT)) { rotation.y += speed + 1; }
	if (input->PushKey(DIK_LEFT)) { rotation.y -= speed + 1; }

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
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);
	
	velocity = XMVector3TransformNormal(velocity, object->GetMatWorld());
	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//�������s����
	newBullet->Initialize(position, velocity);

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
