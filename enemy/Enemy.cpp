#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(enemyObj);
	delete(frameObj);
	delete(enemyModel);
	delete(frameModel);
}

void Enemy::Initialize()
{
	//�G�I�u�W�F�b�g
	enemyModel = Model::LoadFromObj("BlueBox");
	enemyObj = Object3d::Create();
	enemyObj->SetModel(enemyModel);
	enemyObj->SetScale({ 0.65f, 0.65f, 0.65f });

	//�g�̃I�u�W�F�b�g
	frameModel = Model::LoadFromObj("ClearBox");
	frameObj = Object3d::Create();
	frameObj->SetModel(frameModel);
	frameObj->SetScale({ 0.66f, 0.66f, 0.66f });

	// ���݂̍��W���擾
	position = enemyObj->GetPosition();
	int x = rand() % 400;
	float x2 = (float)x / 10 - 20;//10�`-10�͈̔�
	int y = rand() % 70;
	float y2 = (float)y / 10;//6~0�͈̔�
	int z = rand() % 400;
	float z2 = (float)z / 10 - 20;//6~0�͈̔�
	position = { x2, y2, z2 };
	// ���W�̕ύX�𔽉f
	enemyObj->SetPosition(position);
	frameObj->SetPosition(position);

}

void Enemy::Update()
{
	UpdateAliveFlag();

	enemyObj->Update();
	frameObj->Update();
}

void Enemy::UpdateAliveFlag()
{
	if (aliveFlag == 1) {
		if (frameFlag == 0) {
			frameTimer++;
		}

		if (frameTimer >= 180) {
			frameFlag = 1;
		}

		enemyTimer++;
		//���b��������
		if (enemyTimer >= 480) {
			aliveFlag = 0;
			frameFlag = 0;
			frameTimer = 0;
			flashingTimer = 0;
		}
	}

	if (aliveFlag == 0) {
		//�����ňʒu�w��
		int x = rand() % 200;
		float x2 = (float)x / 10 - 10;//10�`-10�͈̔�
		int y = rand() % 60;
		float y2 = (float)y / 10;//6�`0�͈̔�
		enemyObj->SetPosition({ x2, y2, 5 });
		frameObj->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		frameFlag = 0;
		frameTimer = 0;
		aliveFlag = 1;
	}
}

void Enemy::Draw()
{
	//�t���O0�̎��ɘg�����\��
	if (frameFlag == 0) {
		frameObj->Draw();
	}
	//�t���O1�œG�\��
	if (frameFlag == 1) {
		if (aliveFlag == 1) {
			enemyObj->Draw();
		}
	}
}
