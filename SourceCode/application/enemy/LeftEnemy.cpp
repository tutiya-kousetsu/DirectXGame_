#include "LeftEnemy.h"
#include "Player.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "ParticleManager.h"

LeftEnemy::LeftEnemy() :LeftEnemy(Model::CreateFromOBJ("greenSquid"))
{
	object->SetScale({ 1.3f, 1.3f, 1.3f });
	particleMan = ParticleManager::GetInstance();
	audio = Audio::GetInstance();
	audio->SoundLoadWave("enHit.wav");
}

LeftEnemy::~LeftEnemy()
{
}

bool LeftEnemy::Initialize(XMFLOAT3 position, XMFLOAT3 rotation)
{
	this->position = position;
	this->rotation = rotation;
	SetRotation(rotation);
	AccessPhase();
	return true;
}

void LeftEnemy::Update()
{
	//�e�̃t���O��false�ɂȂ�����폜����
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
	if (alive) {
		appearance();
		if (!appFlag) {
			Shoot();

		}
		object->SetPosition(position);
	}

	object->Update();
}

void LeftEnemy::appearance()
{
	if (position.x <= -50) {
		position.x += moveX;
		if (position.x >= -50) {
			moveX = 0;
			appFlag = false;
		}
	}
	if (position.z >= 50) {
		position.z -= moveZ;
		if (position.z <= 50) {
			moveZ = 0;
			appFlag = false;
		}
	}
}

void LeftEnemy::LeftShoot()
{
	//player�Ɍ������Ēe����
	{
		assert(this->player);

		XMVECTOR playerPos = player->GetWorldPosition();
		XMVECTOR enemyPos = GetWorldPosition();

		//���x���v�Z
		//��������W�I�܂ł̃x�N�g��
		velocity = {
			playerPos.m128_f32[0] - enemyPos.m128_f32[0],
			playerPos.m128_f32[1] - enemyPos.m128_f32[1],
			playerPos.m128_f32[2] - enemyPos.m128_f32[2]
		};
		//�傫����1�ɂ���(�x�N�g���𐳋K�����ĕԂ��Ă�����֐�)
		velocity = XMVector3Normalize(velocity);
		//�傫����C�ӂ̒l�ɂ���
		velocity = XMVectorScale(velocity, 1.5f);
	}
	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//������
	newBullet->Initialize(position, velocity);

	bullets.push_back(std::move(newBullet));
}

void LeftEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		LeftShoot();
		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

void LeftEnemy::OnCollision()
{
	audio = Audio::GetInstance();
	audio->SoundPlayWave("enHit.wav", false);
	for (int j = 0; j < 100; j++) {
		DirectX::XMFLOAT3 pos = object->GetPosition();
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

	life--;
	if (life == 0) {
		alive = false;

	}
}

void LeftEnemy::Draw()
{
	//�t���O1�œG�\��
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}


XMVECTOR LeftEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos��player��position�������
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void LeftEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}