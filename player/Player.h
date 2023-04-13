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

	~Player();

	bool Initialize() override;

	//更新
	void Update() override;

	//動かしたくない時用更新
	void StopUpdate();

	//チュートリアル用更新
	void TutorialUpdate();

	//移動
	void move(float speed = 0.3f);

	//ジャンプ
	void jump();

	//描画
	void Draw() override;

	//攻撃
	bool Shoot();

	//当たり判定
	void OnCollision();

	//パーティク生成
	void CreateParticle();
	//弾リスト取得
	//const std::unique_ptr<PlayerBullet>& GetBullet() { return bullet; }
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }


	inline void SetLife(int life) { this->life = life; }
	inline const int GetLife() const { return life; }

	inline int GetPhase() { return operatePhase; }
	
public:
	int operatePhase = 0;
	bool moveFlag = false;
	bool jumpOpFlag = false;
	bool shotFlag = false;
private:
	std::list<std::unique_ptr<PlayerBullet>> bullets;
	bool bulFlag = true;
	// マウス
	POINT mousePos{};
	ParticleManager* particleMan = nullptr;
	int32_t shootTimer = 0;

	//攻撃(長押しかどうかのフラグ、タイマー、力)
	int atPower = 1;
	int atTimer = 0;
	bool atFlag = false;
	int mWaiteCount = 0;
	bool canShot = false;
	// ジャンプ
	bool alive = true;
	int life = 7;
	bool jumpFlag = false;
	bool secondFlag = false;
	int jumpCount = 2;
	float g = 0.25;//重力
	float jumpSpeed = 0;
	float speed = 7.0f;	//速度
	float t = 0.01f;		//時間
	float speed2 = 2.55f;	//速度
	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	DirectX::XMVECTOR fallV;
};

