#include "Enemy.h"

Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 0.65f, 0.65f, 0.65f });
}


void Enemy::Initialize()
{
	//Shoot();

	
	AccessPhase();

	// ���݂̍��W���擾
	position = object->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	int z = rand() % 400;
	float z2 = (float)z / 10 - 20;//6~0�͈̔�
	position = { x2, y2, z2 };
	// ���W�̕ύX�𔽉f
	object->SetPosition(position);
	//frameObj->SetPosition(position);
	}

void Enemy::Update()
{

	shootTimer--;

	if (shootTimer < 0) {
		Shoot();

		shootTimer = kShootInterval;
	}

	UpdateAliveFlag();
	if (bullet) {
		bullet->Update();
	}
	object->Update();
}

void Enemy::UpdateAliveFlag()
{
	if (aliveFlag == 1) {
		enemyTimer++;
		//���b��������
		if (enemyTimer >= 480) {
			aliveFlag = 0;
		}
	}

	if (aliveFlag == 0) {
		//�����ňʒu�w��
		int x = rand() % 200;
		float x2 = (float)x / 10 - 10;//10�`-10�͈̔�
		int y = rand() % 60;
		float y2 = (float)y / 10;//6�`0�͈̔�
		object->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		aliveFlag = 1;
	}
}

void Enemy::Draw()
{
	//�t���O1�œG�\��
		if (aliveFlag == 1) {
			object->Draw();
			
		}
		if (bullet) {
			bullet->Draw();
		}
}

void Enemy::Shoot()
{
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(position);
	
	bullet.reset(newBullet);

	//�R���X�g���N�^�ĂԂ�
	//bullet = new EnemyBullet();
	////�������s����
	//bullet->Initialize(position);

}

void Enemy::OnCollision()
{
	aliveFlag = 0;
}

void Enemy::AccessPhase()
{
	shootTimer = kShootInterval;
}