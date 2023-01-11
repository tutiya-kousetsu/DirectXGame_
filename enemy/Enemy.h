#pragma once
#include "Object3d.h"
#include "EnemyBullet.h"
#include "Line.h"
#include <memory>
#include <list>

using namespace DirectX;
//playerの前方宣言
class Player;

class Enemy
{
public://サブクラス
	struct frontEnemy {
		XMFLOAT3 frontPos[4] = {};
		int life = 3;
		std::unique_ptr<Object3d> object;
		bool alive = true;
		inline bool GetAlive() const { return  alive; }
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

	};

	struct leftEnemy {
		XMFLOAT3 leftPos[4] = {};
		int life = 3;
		std::unique_ptr<Object3d> object;
		bool alive = true;
		inline bool GetAlive() const { return  alive; }
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

	};

	struct rightEnemy {
		XMFLOAT3 rightPos[4] = {};
		int life = 3;
		std::unique_ptr<Object3d> object;
		bool alive = true;
		inline bool GetAlive() const { return  alive; }
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

	};

	struct backEnemy {
		XMFLOAT3 backPos[4] = {};
		int life = 3;
		std::unique_ptr<Object3d> object;
		bool alive = true;

		inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
		inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

		inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
		inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

		inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object->SetRotation(rotation); }
		inline const DirectX::XMFLOAT3& GetRotation() const { return object->GetRotation(); }

		inline const DirectX::XMMATRIX& GetMatRotation() const { return object->GetMatRot(); }
		inline void SetMatRotation(const DirectX::XMMATRIX& matRot) { object->SetMatRotation(matRot); }
		inline const DirectX::XMMATRIX& GetMatWorld() const { return object->GetMatWorld(); }

	};

public:
	//コンストラクタ
	Enemy();

	~Enemy();

	//初期化
	void Initialize();

	//更新
	void Update();

	//描画
	void Draw();

	void Shoot();

	//衝突時に呼び出される関数
	void OnCollision();

public:

	static const int kShootInterval = 120;

	//弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullet() { return bullets; }

	void AccessPhase();

	void SetPlayer(Player* player) { this->player = player; }

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

private:
	int32_t shootTimer = 0;
	int enemyTimer = 0;
	Player* player = nullptr;
	Line* line = nullptr;
	XMFLOAT3 position;
	XMFLOAT3 linePos;
	XMVECTOR velocity;
	std::list<std::unique_ptr<EnemyBullet>> bullets;
	bool bulFlag = true;

	frontEnemy* frontEne = nullptr;
	leftEnemy* leftEne = nullptr;
	rightEnemy* rightEne = nullptr;
	backEnemy* backEne = nullptr;
};

