#include "FrontEnemy.h"
#include "Player.h"

FrontEnemy::FrontEnemy()
{

	SetScale({ 1.0f, 1.0f, 1.0f });

	// ���݂̍��W���擾
	position = GetPosition();
	int x = rand() % 700;
	float x2 = (float)x / 10 - 35;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	int z = rand() % 700;
	//float z2 = (float)z / 10 - 35;//6~0�͈̔�
	position = { x2, 35, 30 };
	// ���W�̕ύX�𔽉f
	SetPosition(position);
}

void FrontEnemy::Initialize()
{
	AccessPhase();
}

void FrontEnemy::Update()
{
	if (alive) {
		appearance();
		//�G���~�܂�����t���O�𗧂ĂĒe�������n�߂�
		if (shootFlag) {
			Shoot();
		}
		object->SetPosition(position);
	}

	object->Update();
}

void FrontEnemy::appearance()
{
	//�`�悳�ꂽ��A�G�������_���Ō��߂��ʒu�̍����܂ł��낷
	position.y -= moveY;
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	if (position.y <= y2) {
		moveY = 0;
		shootFlag = true;
	}
}

void FrontEnemy::FrontShoot()
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
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//�������s����
	newBullet->Initialize(position, velocity);
	//�e��o�^����
	bullets.push_back(std::move(newBullet));
}

void FrontEnemy::Shoot()
{
	shootTimer--;
	if (shootTimer < 0) {
		FrontShoot();
		//Shoot();

		shootTimer = kShootInterval;
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
	}
}

XMVECTOR FrontEnemy::GetWorldPosition()
{
	XMVECTOR worldPos;
	//worldPos��player��position�������
	worldPos.m128_f32[0] = position.x;
	worldPos.m128_f32[1] = position.y;
	worldPos.m128_f32[2] = position.z;

	return worldPos;
}

void FrontEnemy::AccessPhase()
{
	shootTimer = kShootInterval;
}