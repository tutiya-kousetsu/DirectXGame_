#include "BackEnemy.h"
#include "Player.h"

BackEnemy::BackEnemy()
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
	position = { x2, y2, -35 };
	// ���W�̕ύX�𔽉f
	SetPosition(position);
}

void BackEnemy::Initialize()
{
	AccessPhase();
}

void BackEnemy::Update()
{
	if (alive) {
		shootTimer--;
		if (shootTimer < 0) {
			BackShoot();
			//Shoot();

			shootTimer = kShootInterval;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Update();
		}

		position.x += move;
		if (position.x >= 35) {
			move = move * -1;
		}
		if (position.x <= -35) {
			move = move * -1;
		}
		object->SetPosition(position);
	}

	object->Update();
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
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//�������s����
	newBullet->Initialize(position, velocity);
	//�e��o�^����
	bullets.push_back(std::move(newBullet));
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