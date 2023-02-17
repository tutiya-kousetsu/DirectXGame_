//#pragma once
//#include "GameObject.h"
//#include "EnemyObject.h"
//#include "Object3d.h"
//#include "EnemyBullet.h"
//#include "Line.h"
//#include "CollisionInfo.h"
//#include "CollisionPrimitive.h"
//#include <memory>
//#include <list>
//
//using namespace DirectX;
////player�̑O���錾
//class Player;
//class ParticleManager;
//
//class Enemy : public EnemyObject
//{
//public:
//	/// <summary>
//	/// 3D�I�u�W�F�N�g����
//	/// </summary>
//	/// <returns>�C���X�^���X</returns>
//	static Enemy* Create(Model* model = nullptr);
//
//public:
//	//�R���X�g���N�^
//	//Enemy();
//
//	~Enemy();
//
//	bool Initialize() override;
//
//	//�`��
//	void Draw();
//
//	//�Փˎ��ɌĂяo�����֐�
//	//void OnCollision();
//
//public:
//
//	static const int kShootInterval = 120;
//
//	//�e���X�g�擾
//	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }
//
//	inline bool GetAlive() const { return  alive; }
//	inline void SetAlive(bool alive) { this->alive = alive; }
//
//protected:
//	int32_t shootTimer = 0;
//	int enemyTimer = 0;
//	int enemyPopFlag = 0;
//	Player* player = nullptr;
//	ParticleManager* particleMan = nullptr;
//	Line* line = nullptr;
//	std::list<std::unique_ptr<EnemyBullet>> bullets;
//	bool bulFlag = true;
//	int life = 2;
//	float move = 0.2f;
//	float moveY = 0.2f;
//	bool appFlag = true;
//	std::unique_ptr<EnemyObject> object;
//	int alive = true;
//};
//
