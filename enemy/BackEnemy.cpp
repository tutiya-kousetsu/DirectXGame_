#include "BackEnemy.h"
#include "Player.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"
#include "CollisionManager.h"
#include "ParticleManager.h"

BackEnemy::~BackEnemy()
{
}

bool BackEnemy::Initialize()
{
	object.reset(new EnemyObject());
	object->Initialize(Model::CreateFromOBJ("BlueBox"));

	SetScale({ 1.0f, 1.0f, 1.0f });
	particleMan = ParticleManager::GetInstance();
	// ���݂̍��W���擾
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	int z = rand() % 700;
	//float z2 = (float)z / 10 - 35;//6~0�͈̔�
	position = { x2, 35, -35 };
	// ���W�̕ύX�𔽉f
	SetPosition(position);
	AccessPhase();
	return true;
}

void BackEnemy::Update()
{
	if (alive) {
		appearance();

		if (!appFlag) {
			Shoot();

			position.x += move;
			if (position.x >= 35) {
				move = move * -1;
			}
			if (position.x <= -35) {
				move = move * -1;
			}
			position.y += bMoveY;
			if (position.y >= 7) {
				bMoveY = bMoveY * -1;
			}
			if (position.y <= 0) {
				bMoveY = bMoveY * -1;
			}
		}
		object->SetPosition(position);
	}

	object->Update();
}

void BackEnemy::appearance()
{
	//�`�悳�ꂽ��A�G�������_���Ō��߂��ʒu�̍����܂ł��낷
	position.y -= moveY;
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	if (position.y <= y2) {
		moveY = 0;
		appFlag = false;
	}
}

void BackEnemy::BackShoot()
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

		//�W�I�Ɍ�����
		float rotx = atan2f(velocity.m128_f32[1], velocity.m128_f32[2]);
		float roty = atan2f(velocity.m128_f32[0], velocity.m128_f32[2]);
	}
	//�R���X�g���N�^�ĂԂ�
	EnemyBullet* newBullet = new EnemyBullet();
	//������
	newBullet->Initialize(position, velocity);

	bullets.reset(newBullet);
}

void BackEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		BackShoot();
		shootTimer = kShootInterval;
	}
	if (bullets) {
		bullets->Update();
	}
}

void BackEnemy::Draw()
{
	//�t���O1�œG�\��
	if (alive) {
		object->Draw();
		if(bullets) {
			bullets->Draw();
		}
	}
}

void BackEnemy::OnCollision()
{
}


XMVECTOR BackEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos��player��position�������
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void BackEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}