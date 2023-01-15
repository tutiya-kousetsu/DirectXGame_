#include "Enemy.h"
#include "Player.h"
Enemy::Enemy() :Enemy(Model::LoadFromObj("BlueBox"))
{
	object->SetScale({ 1.0f, 1.0f, 1.0f });
}

Enemy::~Enemy()
{

}

void Enemy::Draw()
{
	//フラグ1で敵表示
	if (alive) {
		object->Draw();
		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
			bullet->Draw();
		}
	}
}

void Enemy::OnCollision()
{
	alive = false;
}