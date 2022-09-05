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

					flashingTimer++;
					//0�ȏ�120�ȉ��Ȃ�\��
					if (flashingFlag == 0 && 0 < flashingTimer < 120) {
						flashingFlag = 1;
					}
					//120�ȏ�240�ȉ��Ȃ��\��
					else if (flashingFlag == 1 && 120 < flashingTimer < 240) {
						flashingFlag = 0;
					}
					else if (flashingFlag == 0 && 240 < flashingTimer < 360) {
						flashingFlag = 1;
					}
					else if (flashingFlag == 1 && 360 < flashingTimer < 480) {
						flashingFlag = 0;
					}
					else if (flashingFlag == 0 && 480 < flashingTimer < 600) {
						flashingFlag = 1;
					}
			}
			
			if (frameTimer >= 600) {

				frameFlag = 1;
			}
			
			enemyTimer++;
			// ���݂̍��W���擾
			XMFLOAT3 position = enemyObj[i]->GetPosition();
			// ���W�̕ύX�𔽉f
			enemyObj[i]->SetPosition(position);
			frameObj[i]->SetPosition(position);
			//��ʂ͎��܂ōs������
			if (enemyTimer >= 720) {
				aliveFlag[i] = 0;
				frameFlag = 0;
				frameTimer = 0;
				flashingTimer = 0;
				//flashingFlag = 0;
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
			XMFLOAT3 position = enemyObj[i]->GetPosition();
			enemyObj[0]->SetPosition({ x2, y2, z2 });
			enemyObj[1]->SetPosition({ x2, y2, z2 });
			enemyObj[2]->SetPosition({ x2, y2, z2 });
			enemyTimer = 0;
			flashingTimer = 0;
			flashingFlag = 0;
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
			if (flashingFlag == 1) {
				frameObj[i]->Draw();
			}
		}
		//�t���O1�œG�\��
		if (frameFlag == 1) {
			if (aliveFlag[i] == 1) {
				enemyObj[i]->Draw();

			}
		}
	}
}
