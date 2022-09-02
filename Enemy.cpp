#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	for (int i = 0; i < 3; i++) {
		delete(enemyObj[i]);
		delete(frameObj[i]);
	}
	delete(enemyModel);
	delete(frameModel);
}

void Enemy::Initialize()
{
	for (int i = 0; i < 3; i++) {
		//�G�I�u�W�F�b�g
		enemyModel = Model::LoadFromObj("BlueBox");
		enemyObj[i] = Object3d::Create();
		enemyObj[i]->SetModel(enemyModel);
		enemyObj[i]->SetScale({ 0.65f, 0.65f, 0.65f });

		//�g�̃I�u�W�F�b�g
		frameModel = Model::LoadFromObj("ClearBox");
		frameObj[i] = Object3d::Create();
		frameObj[i]->SetModel(frameModel);
		frameObj[i]->SetScale({ 0.66f, 0.66f, 0.66f });
	}
}

void Enemy::Update()
{

	for (int i = 0; i < 3; i++) {
		if (aliveFlag[i] == 1) {
			if (frameFlag == 0) {
				frameTimer++;
			}
			if (frameTimer >= 120) {
				frameFlag = 1;
			}
			enemyTimer++;
			// ���݂̍��W���擾
			XMFLOAT3 position = enemyObj[i]->GetPosition();
			XMFLOAT3 framePos = frameObj[i]->GetPosition();
			// ���W�̕ύX�𔽉f
			enemyObj[i]->SetPosition(position);
			frameObj[i]->SetPosition(position);
			//��ʂ͎��܂ōs������
			if (enemyTimer >= 600) {
				aliveFlag[i] = 0;
				frameFlag = 0;
				frameTimer = 0;
			}
		}

		if (aliveFlag[i] == 0) {
			//�����ňʒu�w��
			int x = rand() % 160;
			float x2 = (float)x / 10 - 8;//8�`-8�͈̔�
			int y = rand() % 160;
			float y2 = (float)y / 10 - 8;//8�`-8�͈̔�
			int z = rand() % 160;
			float z2 = (float)z / 10 - 8;//8�`-8�͈̔�
			// ���W�̕ύX�𔽉f
			enemyObj[0]->SetPosition({ x2, y2, z2 });
			enemyObj[1]->SetPosition({ x2, y2, z2 });
			enemyObj[2]->SetPosition({ x2, y2, z2 });
			enemyTimer = 0;
			
			aliveFlag[i] = 1;
		}

		enemyObj[i]->Update();
		frameObj[i]->Update();
	}
}

void Enemy::Draw()
{
	for (int i = 0; i < 3; i++) {
		//�t���O0�̎��ɘg�����\��
		if (frameFlag == 0) {
			frameObj[i]->Draw();
		}
		//�t���O1�œG�\��
		if (frameFlag == 1) {
			if (aliveFlag[i] == 1) {
				enemyObj[i]->Draw();

			}
		}
	}
}
