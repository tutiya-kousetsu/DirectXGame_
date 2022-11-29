#include "Enemy.h"

Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	XMVECTOR worldPos;

	worldPos.m128_f32[0] = object->GetPosition().x;
	worldPos.m128_f32[1] = object->GetPosition().y;
	worldPos.m128_f32[2] = object->GetPosition().z;

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	AccessPhase();

	// ���݂̍��W���擾
	position = object->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	position = { x2, y2, 35 };

	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
}

void Enemy::Update()
{
	if (alive) {
		shootTimer--;
		if (shootTimer < 0) {
			Shoot();

			shootTimer = kShootInterval;
		}
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
				bullet->Update();
		}
	}
	object->Update();
}

void Enemy::UpdateAliveFlag()
{
	//if (aliveFlag == 1) {
	//	//enemyTimer++;
	//	//���b��������
	//	if (enemyTimer >= 300) {
	//		aliveFlag = 0;
	//	}
	//	
	//}

	//if (aliveFlag == 0) {
	//	//�����ňʒu�w��
	//	//int x = rand() % 200;
	//	//float x2 = (float)x / 10 - 10;//10�`-10�͈̔�
	//	//int y = rand() % 60;
	//	//float y2 = (float)y / 10;//6�`0�͈̔�
	//	//object->SetPosition({ x2, y2, 5 });
	//	//enemyTimer = 0;
	//	//aliveFlag = 1;
	//	//for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
	//	//	bullet->SetPos({ x2, y2, 5 });
	//	//}
	//}
}

void Enemy::Draw()
{
	//�t���O1�œG�\��
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}

void Enemy::Shoot()
{
	assert(this->player);

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	

	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//�������s����
	newBullet->Initialize(position);
	//�e��o�^����
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision()
{
	alive = false;
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bulFlag = bullet->GetAlive();
		bulFlag = false;
	}
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}