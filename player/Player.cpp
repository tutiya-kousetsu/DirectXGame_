#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
Player* Player::Create(Model* model)
{
	//3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
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

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	Object3d::SetPosition({ 0,0,0 });
	//�R���C�_�[�̒ǉ�
	float radius = 0.6f;
	//���a�����������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();

	move();
	jump();
	playerRot();
	if (input->TriggerMouseLeft()) {
		Shoot();
	}

	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}
	//particleMan->Update();
	Object3d::Update();

}

void Player::move(float speed)
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	DirectX::XMFLOAT3 position = Object3d::GetPosition();
	float moveSpeed = 0.3f;
	// �O�����Ɖ������̒P�ʃx�N�g�������
	XMVECTOR forwardVec = XMVectorSet(0, 0, 1, 1);
	XMVECTOR horizontalVec = XMVectorSet(1, 0, 0, 1);
	// �v���C���[�̉�]�ɍ��킹�ĉ�]������(�O��)
	forwardVec = XMVector3Rotate(forwardVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// �v���C���[�̉�]�ɍ��킹�ĉ�]������(��)
	horizontalVec = XMVector3Rotate(horizontalVec, XMQuaternionRotationRollPitchYaw(
		XMConvertToRadians(Object3d::GetRotation().x),
		XMConvertToRadians(Object3d::GetRotation().y),
		XMConvertToRadians(Object3d::GetRotation().z)));
	// �傫����moveSpeed�ɂ���
	forwardVec = XMVectorScale(forwardVec, moveSpeed);
	horizontalVec = XMVectorScale(horizontalVec, moveSpeed);

	if (input->PushKey(DIK_W)) {
		position.x += forwardVec.m128_f32[0];
		position.x += forwardVec.m128_f32[1];
		position.z += forwardVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_S)) {
		position.x -= forwardVec.m128_f32[0];
		position.x -= forwardVec.m128_f32[1];
		position.z -= forwardVec.m128_f32[2];
	}

	if (input->PushKey(DIK_D)) {
		position.x += horizontalVec.m128_f32[0];
		position.x += horizontalVec.m128_f32[1];
		position.z += horizontalVec.m128_f32[2];
	}
	else if (input->PushKey(DIK_A)) {
		position.x -= horizontalVec.m128_f32[0];
		position.x -= horizontalVec.m128_f32[1];
		position.z -= horizontalVec.m128_f32[2];
	}
	// ���W�̕ύX�𔽉f
	Object3d::SetPosition(position);

}

void Player::jump()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = Object3d::GetPosition();
	//�d��
	//position.y -= g;

	//�}�E�X�̉E���N���b�N������W�����v
	if (!onGround) {

		position.y += jumpSpeed;
		//�W�����v�̑��x��0.04�������Ă���
		jumpSpeed -= 0.05f;
	}
	//�W�����v�t���O��1�ɂȂ�����
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		//�W�����v�̍���
		jumpSpeed = 1.0f;
	}
	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 5.0f + adsDistance)) {
			onGround = true;
			//position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 4.0f);
			//position.y += g;
			// �s��̍X�V�Ȃ�
			Object3d::Update();
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else {
			onGround = false;
			fallV = {};
		}
	}
	// �������
	else if (fallV.m128_f32[1] <= 0.0f) {
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 5.5f)) {
			// ���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 5.5f);
			// �s��̍X�V�Ȃ�
			Object3d::Update();
		}
	}
	//�|�W�V������0�ɂȂ�����W�����v�t���O��؂�
	/*if (position.y <= 0) {
		jumpFlag = false;
	}*/
	// ��������
	//if (input->TriggerMouseRight() && !onGround || input->TriggerKey(DIK_SPACE) && !onGround) {
	//	// �����������x
	//	const float fallAcc = -0.01f;
	//	const float fallVYMin = -0.5f;
	//	// ����
	//	fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);
	//	// �ړ�
	//	position.x += fallV.m128_f32[0];
	//	position.y += fallV.m128_f32[1];
	//	position.z += fallV.m128_f32[2];
	//}
	//// �W�����v����
	//else if (input->TriggerKey(DIK_SPACE)) {
	//	onGround = false;
	//	const float jumpVYFist = 1.2f;
	//	fallV = { 0, jumpVYFist, 0, 0 };
	//}
	// ���W�̕ύX�𔽉f
	///Object3d::SetPosition(position);
	

	// �ڒn���
	//if (onGround) {
	//	// �X���[�Y�ɍ������ׂ̋z������
	//	const float adsDistance = 0.2f;
	//	// �ڒn���ێ�
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//		// �s��̍X�V�Ȃ�
	//		Object3d::Update();
	//	}
	//	// �n�ʂ��Ȃ��̂ŗ���
	//	else {
	//		onGround = false;
	//		fallV = {};
	//	}
	//}
	//// �������
	//else if (fallV.m128_f32[1] <= 0.0f) {
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
	//		// ���n
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//		// �s��̍X�V�Ȃ�
	//		Object3d::Update();
	//	}
	//}
}

void Player::playerRot()
{
	//Input* input = Input::GetInstance();
	//Input::MouseMove mouseMove = input->GetMouseMove();
	//float dy = mouseMove.lX * scaleY;
	//angleY = -dy * XM_PI;

	//if (angleY) { rotation.y += speed + 1; }
	//if (-angleY) { rotation.y -= speed + 1; }

	//Object3d::SetRotation(rotation);
}

void Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());
	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//�������s����
	newBullet->Initialize(position, velocity);

	//�e��o�^����
	bullets.push_back(std::move(newBullet));

}

void Player::OnCollision(const CollisionInfo& info)
{
	//for (int i = 0; i < 1; i++) {
	//	//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
	//	const float md_pos = 5.0f;
	//	DirectX::XMFLOAT3 pos = Object3d::GetPosition();
	//	//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
	//	const float md_vel = 0.1f;
	//	DirectX::XMFLOAT3 vel{};
	//	vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
	//	//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
	//	DirectX::XMFLOAT3 acc{};
	//	const float rnd_acc = 0.005f;
	//	acc.y = -(float)rand() / RAND_MAX * rnd_acc;

	//	//�ǉ�
	//	particleMan->Add(60, DirectX::XMFLOAT3(info.inter.m128_f32), vel, acc, 0.0f, 1.0f);
 //	}
}

void Player::FloorCollision()
{
	//�͈͑I��
	if (position.x <= 33.f && position.x >= -33.f
		&& position.z <= 33.f && position.z >= -33.f
		) {
		position.y += g;
	}
	else {
		onGround = true;
	}
	Object3d::SetPosition(position);
}

void Player::Draw()
{
	if (alive) {
		Object3d::Draw();
		//particleMan->Draw();
	}
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

}

XMVECTOR Player::GetWorldPosition()
{
	XMVECTOR worldPos{};

	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}
