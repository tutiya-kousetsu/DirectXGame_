#include "Enemy.h"

Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 0.65f, 0.65f, 0.65f });
}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	AccessPhase();

	// ���݂̍��W���擾
	//position = object->GetPosition();
	//int x = rand() % 400;
	//float x2 = (float)x / 10 - 20;//10�`-10�͈̔�
	//int y = rand() % 70;
	//float y2 = (float)y / 10;//6~0�͈̔�
	//position = { 5, 5, 5 };

	// ���W�̕ύX�𔽉f
	//object->SetPosition(position);
	}

void Enemy::Update()
{
	shootTimer--;

	if (shootTimer < 0) {
		Shoot();

		shootTimer = kShootInterval;
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->SetPos(position);
		}
	}
	for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
		bullet->Update();
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
		if (aliveFlag == 1) {
			object->Draw();
			for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
				bullet->Draw();
			}
		}
		
}

void Enemy::Shoot()
{
	//�R���X�g���N�^�ĂԂ�
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//�������s����
	newBullet->Initialize(position);
	//�e��o�^����
	bullets.push_back(std::move(newBullet));
}

void Enemy::OnCollision()
{
	aliveFlag = 0;
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}