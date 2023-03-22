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
	Object3d::SetPosition({ 0,0,0 });
	Object3d::SetScale({ 0.9f,0.9f,0.9f });
	//�R���C�_�[�̒ǉ�
	float radius = 2.0f;
	//���a�����������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(DirectX::XMVECTOR({ 0, radius, 0, 0 }), radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
	return true;
}

void Player::Update()
{
	//�e�̃t���O��false�ɂȂ�����폜����
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
	});
	move();
	jump();
	Input* input = Input::GetInstance();
		if (input->TriggerMouseLeft()) {
			Shoot();
		}
	for (std::unique_ptr<PlayerBullet>& bul : bullets) {
		bul->Update();
	}
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

	//��������
	if (!onGround) {
		//�������̉����x
		const float fallAcc = -0.021f;
		const float fallVYMin = -0.5f;
		//����
		fallV.m128_f32[1] = max(fallV.m128_f32[1] + fallAcc, fallVYMin);

		//�ړ�
		if (position.y >= -5.0f) {
			position.x += fallV.m128_f32[0];
			position.y += fallV.m128_f32[1];
			position.z += fallV.m128_f32[2];
		}
	}
	//�W�����v����
	else if (input->TriggerMouseRight() || input->TriggerKey(DIK_SPACE)) {
		onGround = false;
		const float jumpVYFist = 0.56f;
		fallV = { 0, jumpVYFist, 0, 0 };
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

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
			const float threshold = cosf(XMConvertToRadians(0.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}
			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.5f + adsDistance)) {
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.5f);
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
		if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.5f)) {
			// ���n
			onGround = true;
			position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.5f);

		}
	}
	// ���[���h�s��X�V
	UpdateWorldMatrix();
	collider->Update();

	// �s��̍X�V�Ȃ�
	Object3d::Update();
}

bool Player::Shoot()
{
	const float kBulletSpeed = 1.0f;
	XMVECTOR velocity = XMVectorSet(0, 0, kBulletSpeed, 1);

	velocity = XMVector3TransformNormal(velocity, Object3d::GetMatWorld());

	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
	//������
	newBullet->Initialize(position, velocity);

	bullets.push_back(std::move(newBullet));
	return true;
}


void Player::OnCollision(const CollisionInfo& info)
{

}

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
	if (alive) {
		Object3d::Draw();
	}
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
