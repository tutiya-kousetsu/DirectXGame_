//#include "Enemy.h"
//#include "Player.h"
//#include "BaseCollider.h"
//#include "CollisionManager.h"
//#include "ParticleManager.h"
////Enemy::Enemy()
////{
////	//object->SetScale({ 1.0f, 1.0f, 1.0f });
////	//object->SetPosition(pos);
////}
//
//Enemy* Enemy::Create(Model* model)
//{
//	//3Dオブジェクトのインスタンスを生成
//	Enemy* instance = new Enemy();
//	if (instance == nullptr) {
//		return nullptr;
//	}
//
//	//初期化
//	if (!instance->Initialize()) {
//		delete instance;
//		assert(0);
//	}
//	//モデルのセット
//	if (model) {
//		instance->SetModel(model);
//	}
//
//	return instance;
//
//	return nullptr;
//}
//
//Enemy::~Enemy()
//{
//	if (collider) {
//		//コリジョンマネージャから登録を解除
//		CollisionManager::GetInstance()->RemoveCollider(collider);
//		delete collider;
//	}
//}
//
//bool Enemy::Initialize()
//{
//	if (!Object3d::Initialize()) {
//		return false;
//	}
//	particleMan = ParticleManager::GetInstance();
//	Object3d::SetScale({ 3.01f, 3.01f, 3.01f });
//
//	return true;
//}
//
//void Enemy::Draw()
//{
//	//フラグ1で敵表示
//	if (alive) {
//		object->Draw();
//		for (std::unique_ptr<EnemyBullet>& bullet : bullets) {
//			bullet->Draw();
//		}
//	}
//}
//
////void Enemy::OnCollision()
////{
////	for (int j = 0; j < 100; j++) {
////		XMFLOAT3 pos = object->GetPosition();
////		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
////		const float md_vel = 0.20f;
////		XMFLOAT3 vel{};
////		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
////		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
////		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
////		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
////		XMFLOAT3 acc{};
////		const float rnd_acc = 0.005f;
////		acc.y = -(float)rand() / RAND_MAX * rnd_acc;
////		//追加
////		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f);
////
////	}
////
////	life--;
////	if (life == 0) {
////		alive = false;
////		
////	}
////}