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
}

void Enemy::Update()
{

	if (aliveFlag == 1) {

		if (frameFlag == 0) {
			frameTimer++;
		}

		if (frameTimer >= 240) {
			frameFlag = 1;
		}

		enemyTimer++;
		// ���݂̍��W���擾
		position = enemyObj->GetPosition();
		position.z = 5;
		// ���W�̕ύX�𔽉f
		enemyObj->SetPosition(position);
		frameObj->SetPosition(position);
		//��ʂ͎��܂ōs������
		if (enemyTimer >= 540) {
			aliveFlag = 0;
			frameFlag = 0;
			frameTimer = 0;
			flashingTimer = 0;
			//flashingFlag = 0;
		}
	}

	if (aliveFlag == 0) {
		//�����ňʒu�w��
		int x = rand() % 160;
		float x2 = (float)x / 10 - 8;//8�`-8�͈̔�
		int y = rand() % 160;
		float y2 = (float)y / 10 - 8;//8�`-8�͈̔�
		enemyObj->SetPosition({ x2, y2, 5 });
		enemyTimer = 0;
		flashingTimer = 0;
		flashingFlag = 0;
		aliveFlag = 1;
	}
	enemyObj->Update();
	frameObj->Update();
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
