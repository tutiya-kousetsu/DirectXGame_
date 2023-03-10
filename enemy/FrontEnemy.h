#pragma once
#include "EnemyBullet.h"
#include "Object3d.h"
#include "GameObject.h"
#include "EnemyObject.h"
#include <memory>
#include <list>

class Player;
class ParticleManager;

class FrontEnemy : public GameObject
{
public:
	using GameObject::GameObject;

public:
	FrontEnemy();

	~FrontEnemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize(XMFLOAT3 position);

	//�X�V
	void Update();
	//�o��
	void appearance();
	//�e�̏���
	void FrontShoot();

	void Shoot();

	void OnCollision();

	//�`��
	void Draw();

	//���[���h���W���擾
	XMVECTOR GetWorldPosition();

public:

	static const int kShootInterval = 100;
	//�e���X�g�擾
	const std::unique_ptr<EnemyBullet>& GetBullet() { return bullet; }


	void AccessPhase();
	void SetPlayer(Player* player) { this->player = player; }

	/*inline bool GetAlive() const { return alive[10]; }
	inline void SetAlive(bool alive[10] ) { this->alive[10] = alive[10]; }

	inline void SetPosition(const DirectX::XMFLOAT3& position) { object[10]->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object[10]->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object[10]->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object[10]->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object[10]->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return object[10]->GetRotation(); }

	inline const DirectX::XMMATRIX& GetMatRotation() const { return object[10]->GetMatRot(); }
	inline void SetMatRotation(const DirectX::XMMATRIX& matRot) { object[10]->SetMatRotation(matRot); }
	inline const DirectX::XMMATRIX& GetMatWorld() const { return object[10]->GetMatWorld(); }*/
private:
	Player* player = nullptr;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	float resurrectionTimer = 0;
	int aliveCount = 0;
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	float moveZ = 0.05f;
	//bool appFlag[11] = {};
	//bool alive[11] = {true,true,true,true,true,true,true,true ,true,true,true};
	ParticleManager* particleMan = nullptr;
	//std::list<std::unique_ptr<EnemyBullet>> bullets;
	std::unique_ptr<EnemyBullet> bullet;
	//Object3d* object[11] = {};
	//Model* model[11] = {};
};