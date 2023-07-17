#pragma once
#include "Object3d.h"
#include "Fbx_Object3d.h"
#include "Model.h"
#include "EnemyBullet.h"
#include "Audio.h"
#include <DirectXMath.h>
#include <memory>

class Player;
class ParticleManager;

class BaseEnemy
{
protected:
	std::unique_ptr<Object3d> object;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	ParticleManager* particleMan = nullptr;
	Player* player = nullptr;
	Audio* audio = nullptr;
	bool alive = true;
	DirectX::XMFLOAT3 position;
	XMVECTOR velocity;
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	int enemyPopFlag = 0;
	bool bulFlag = true;
	int life = 2;
	float move = 0.2f;
	float moveY = 0.2f;
	float moveZ = 0.05f;
	float moveX = 0.05f;
	int kShootInterval = 100;
	bool appFlag = true;

public:

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	inline const DirectX::XMMATRIX& GetMatRotation() const { return object->GetMatRot(); }
	inline void SetMatRotation(const DirectX::XMMATRIX& matRot) { object->SetMatRotation(matRot); }
	inline const DirectX::XMMATRIX& GetMatWorld() const { return object->GetMatWorld(); }

	//弾リスト取得
	const std::list < std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	BaseEnemy(Model* model, const DirectX::XMFLOAT3& position = { 0,0,0 });

	virtual void Update();

	virtual void Shoot(XMFLOAT3 position);

	virtual void OnCollision();

	virtual void Draw();

	virtual void SetPlayer(Player* player) { this->player = player; }
	virtual void AccessPhase();

	//ワールド座標を取得
	virtual XMVECTOR GetWorldPosition();
};
