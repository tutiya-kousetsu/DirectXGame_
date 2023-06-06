#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "EnemyBullet.h"
#include "GameObject.h"
#include "Shake.h"
#include "Input.h"
#include "FollowingCamera.h"
#include "Framework.h"
#include <memory>
#include <list>
#include <forward_list>
#include <DirectXMath.h>
class ParticleManager;

using namespace DirectX;

class Player : public Object3d
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
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

	void Mouse();

	//移動
	void move(float speed = 0.3f);

	//ジャンプ
	void jump();

	//スケールを小さくする
	void ScaleSmall();

	void ScaleLarge();

	//描画
	void Draw(bool flag = false);

	//チュートリアル用更新
	void TutorialUpdate();

	//チュートリアル用描画
	void TutorialDraw(bool flag);
	//攻撃
	bool Shoot();

	//当たったらシェイク掛ける(麻痺)
	void Numb(bool flag = false);

	//当たり判定
	void OnCollision(int i);

	//パーティク生成
	void CreateParticle();
	//弾リスト取得
	//const std::unique_ptr<PlayerBullet>& GetBullet() { return bullet; }
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullet() { return bullets; }
	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	inline void SetPhaseFlag(bool phaseF) { this->phaseFlag = phaseF; }

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
	std::unique_ptr<EnemyBullet> eb;
	bool bulFlag = true;
	// マウス
	POINT mousePos{};
	//Input* input = nullptr;
	ParticleManager* particleMan = nullptr;
	int32_t shootTimer = 0;
	std::unique_ptr<Framework> frame;
	std::unique_ptr<Object3d> numb;
	XMFLOAT3 numbPos;
	//攻撃(長押しかどうかのフラグ、タイマー、力)
	int atPower = 1;
	int atTimer = 0;
	bool atFlag = false;
	int mWaiteCount = 0;
	bool canShot = false;
	bool phaseFlag = false;
	// ジャンプ
	bool alive = true;
	int life = 8;
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
	XMVECTOR fallV;
	//シェイク用
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
	// カメラ関係
	bool dirty = false;
	float angleX = 0;
	float angleY = 0;
	float scaleX = 0.35f / (float)WinApp::window_width;
	float scaleY = 0.35f / (float)WinApp::window_height;
	bool viewDirty = false;
	float distance = 1.0f;
	XMMATRIX matRot = DirectX::XMMatrixIdentity();
	std::unique_ptr<FollowingCamera> camera;
};

