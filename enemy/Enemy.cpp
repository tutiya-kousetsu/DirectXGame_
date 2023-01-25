#include "Enemy.h"
#include "Player.h"
#include "ParticleManager.h"
Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
	particleMan = ParticleManager::GetInstance();
}

Enemy::~Enemy()
{

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
	bullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return !bullet->GetAlive();
		});
}

void Enemy::OnCollision()
{
	for (int j = 0; j < 100; j++) {
		XMFLOAT3 pos = object->GetPosition();
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.20f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float rnd_acc = 0.005f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);

	}

	life -= 3;
	if (life == 0) {
		alive = false;
	}
}