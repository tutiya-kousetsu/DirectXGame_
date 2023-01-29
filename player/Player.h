#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>
#include <forward_list>
class ParticleManager;

using namespace DirectX;

class Player : public Object3d
{
public:
	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns>インスタンス</returns>
	static Player* Create(Model* model = nullptr);
public:
	//コンストラクタ
	//Player();

	bool Initialize() override;

	//更新
	void Update() override;

	//移動
	void move(float speed = 0.3f);

	//ジャンプ
	void jump();

	//回転
	void playerRot();

	//描画
	void Draw() override;

	//攻撃
	void Shoot();

	void OnCollision(const CollisionInfo& info) override;

	void FloorCollision();

	//弾リスト取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	bool bulFlag = true;
	// マウス
	POINT mousePos{};
	ParticleManager* particleMan = nullptr;
	// ジャンプ
	bool alive = true;
	bool jumpFlag = false;
	bool secondFlag = false;
	int jumpCount = 2;
	float g = 0.25;//重力
	float jumpSpeed = 0;
	float speed = 7.0f;	//速度
	float t = 0.01f;		//時間
	float speed2 = 2.55f;	//速度

};

