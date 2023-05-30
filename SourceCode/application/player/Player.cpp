#include "Player.h"
#include "Input.h"
#include "ParticleManager.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Collision.h"
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

Player::~Player()
{
}

bool Player::Initialize()
{
	if (!Object3d::Initialize()) {
		return false;
	}
	particleMan = ParticleManager::GetInstance();

	Object3d::SetRotation({ 0,0,0 });
	Object3d::SetScale({ 0.9f,0.9f,0.9f });
	camera.reset(new FollowingCamera());
	shake.reset(new Shake());
	//�R���C�_�[�̒ǉ�
	float radius = 1.9f;
	//���a�����������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	if (life >= 0) {
		//�e�̃t���O��false�ɂȂ�����폜����
		bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
			return !bullet->GetAlive();
			});
		move();
		jump();

		Input* input = Input::GetInstance();
		if (input->TriggerMouseLeft()) {
			//�t�F�[�Y�؂�ւ����͌��ĂȂ�����
			if (!phaseFlag) {
				Shoot();
			}
		}
		for (std::unique_ptr<PlayerBullet>& bul : bullets) {
			bul->Update();
		}
		Object3d::Update();
	}
	if (life <= 0) {
		alive = false;
	}

	Object3d::SetPosition(position);
}

//�����������Ȃ����p�̃A�b�v�f�[�g
void Player::StopUpdate()
{
	Object3d::Update();
}

//�}�E�X�֐�
void Player::Mouse()
{
	//�C���X�^���X�������Ă���input�N���X���g����悤�ɂ���
	Input* input = Input::GetInstance();

	Input::MouseMove mouseMove = input->GetMouseMove();
	float dy = mouseMove.lX * scaleY;
	angleY = -dy * XM_PI;
	{
		// �ǉ���]���̉�]�s��𐶐�
		XMMATRIX matRotNew = XMMatrixIdentity();
		matRotNew *= XMMatrixRotationY(-angleY);
		// �ݐς̉�]�s�������
		matRot = matRotNew * matRot;

		// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
		XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
		XMVECTOR vUp = { 0.0f, 0.5f, 0.0f, 0.0f };

		// �x�N�g������]
		vTargetEye = XMVector3Transform(vTargetEye, matRot);
		vUp = XMVector3Transform(vUp, matRot);

		XMFLOAT3 target1 = camera->GetTarget();
		camera->SetEye({ target1.x + vTargetEye.m128_f32[0], target1.y + vTargetEye.m128_f32[1], target1.z + vTargetEye.m128_f32[2] });
		camera->SetUp({ vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] });

		// �����_���炸�炵���ʒu�Ɏ��_���W������
		XMFLOAT3 target2 = camera->GetTarget();
		XMFLOAT3 eye = camera->GetEye();
		XMFLOAT3 fTargetEye = { 0.0f, 0.0f, 0.0f };
		//���K��
		fTargetEye.x = eye.x - target2.x;
		fTargetEye.y = eye.y - target2.y;
		fTargetEye.z = eye.z - target2.z;

		//�v���C���[�̉�]
		XMFLOAT3 playerRot = Object3d::GetRotation();
		playerRot.y = atan2f(-fTargetEye.x, -fTargetEye.z);
		playerRot.y *= 180 / XM_PI;
		Object3d::SetRotation({ 0.0f, playerRot.y, 0.0f });
	}
}


//�ړ�
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
		moveFlag = true;
	}
	else if (input->PushKey(DIK_S)) {
		position.x -= forwardVec.m128_f32[0];
		position.x -= forwardVec.m128_f32[1];
		position.z -= forwardVec.m128_f32[2];
		moveFlag = true;
	}

	if (input->PushKey(DIK_D)) {
		position.x += horizontalVec.m128_f32[0];
		position.x += horizontalVec.m128_f32[1];
		position.z += horizontalVec.m128_f32[2];
		moveFlag = true;
	}
	else if (input->PushKey(DIK_A)) {
		position.x -= horizontalVec.m128_f32[0];
		position.x -= horizontalVec.m128_f32[1];
		position.z -= horizontalVec.m128_f32[2];
		moveFlag = true;
	}
	if (moveFlag) {
		operatePhase = 1;
	}
	// ���W�̕ύX�𔽉f
	Object3d::SetPosition(position);

}

//�W�����v
void Player::jump()
{
	Input* input = Input::GetInstance();
	// ���݂̍��W���擾
	position = Object3d::GetPosition();

	//��������
	if (!onGround) {
		//�������̉����x
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//�ړ�
		position.x += fallV.m128_f32[0];
		position.y += fallV.m128_f32[1];
		position.z += fallV.m128_f32[2];
	}
	//�W�����v����
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		jumpOpFlag = true;
		const float jumpVYFist = 0.56f;
		fallV = { 0, jumpVYFist, 0, 0 };
	}
	if (jumpOpFlag) {
		operatePhase = 2;
	}

	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };
			//�r�˕���
			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l�p�x
			const float threshold = cosf(XMConvertToRadians(0.0f));
			//�p�x���ɂ���ēV��A�n�ʂƔ��肳���ꍇ��������
			if (-threshold < cos && cos < threshold) {
				//���������o��
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}

		Sphere* sphere = nullptr;
		//�����o���ɂ��ړ���
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	//�R���C�_�[�X�V
	UpdateWorldMatrix();
	collider->Update();

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_ENEMYS);

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		const float adsDistance = 0.18f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.4f);
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
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.4f)) {
			// ���n
			onGround = true;
		}
	}
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// �s��̍X�V�Ȃ�
	Object3d::Update();
}

//�X�P�[��������������p�֐�
void Player::ScaleSmall()
{
	scale = Object3d::GetScale();
	if (scale.x >= 0.f && scale.y >= 0.f && scale.z >= 0.f) {
		scale.x -= 0.01f;
		scale.y -= 0.01f;
		scale.z -= 0.01f;
		Object3d::SetScale(scale);
	}
}

//�X�P�[����傫������p�֐�
void Player::ScaleLarge()
{
	scale = Object3d::GetScale();
	if (scale.x <= 0.9f && scale.y <= 0.9f && scale.z <= 0.9f) {
		scale.x += 0.01f;
		scale.y += 0.01f;
		scale.z += 0.01f;
		Object3d::SetScale(scale);
	}

}

//�e����
bool Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());

	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//������
	newBullet->Initialize({ position.x - 0.45f, position.y + 0.5f, position.z }, velocity);

	bullets.push_back(std::move(newBullet));
	return true;
}

//����������V�F�C�N�|����(���)
void Player::Numb(bool flag)
{
	if (flag) {
		shake->SetShakeStart(true);
	}
	else {
		shake->SetShakeStart(false);
	}
	//�V�F�C�N�̍ő�l�ŏ��l�Ȃǂ�����
	shake->ShakePos(shakePos.x, 1, -1, 10, 20);
	shake->ShakePos(shakePos.z, 1, -1, 10, 20);
	//�t���O��false��������V�F�C�N���Ƃ���
	if (!shake->GetShakeStart()) {
		shakePos = { 0.0f, 0.0f, 0.0f };
	}
	//�|�W�V�����ɃV�F�C�N�̓����𑫂�
	position.x += shakePos.x;
	position.z += shakePos.z;
	Object3d::SetPosition(position);
}


void Player::OnCollision(int i)
{
	life -= i;
	CreateParticle();
}

//�p�[�e�B�N���̐���
void Player::CreateParticle()
{
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = Object3d::GetPosition();
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.20f;
		DirectX::XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		DirectX::XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);

	}
}

void Player::Draw()
{
	Object3d::Draw();
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Draw();
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

//�`���[�g���A���p�̃A�b�v�f�[�g
void Player::TutorialUpdate()
{
	//�e�̃t���O��false�ɂȂ�����폜����
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});
	move();
	//�t�F�[�Y���ʒu�ɂȂ�Ȃ���jump�o���Ȃ�
	if (operatePhase >= 1) {
		jump();
	}
	Input* input = Input::GetInstance();
	if (operatePhase >= 2) {
		if (input->TriggerMouseLeft()) {
			Shoot();
			shotFlag = true;
		}
		if (shotFlag) {
			operatePhase = 3;
		}
	}
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Update();
	}
	Object3d::Update();
}

//�`���[�g���A���p�̕`��֐�
void Player::TutorialDraw(bool flag)
{
	Object3d::Draw();
	if (!flag) {
		for (std::unique_ptr<PlayerBullet>& bul : bullets) {
			bul->Draw();
		}
	}

}
