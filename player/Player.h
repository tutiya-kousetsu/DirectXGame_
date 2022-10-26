#pragma once
#include "Object3d.h"
#include "PlayerBullet.h"
#include "GameObject.h"
#include <memory>
#include <list>

using namespace DirectX;

class Player : public GameObject
{
public:
	using GameObject::GameObject;

	//コンストラクタ
	Player();

	//更新
	void Update() override;

	//移動
	void move(float speed = 0.2f);

	//ジャンプ
	void jump();

	//描画
	void Draw() override;

	//攻撃
	void Shoot();

private:
	//std::list<std::unique_ptr<PlayerBullet>> bullet;
	XMFLOAT3 position;

	// 標的
	//GameObject* shotTarget;
	PlayerBullet* bullet = nullptr;
	// ジャンプ
	bool jumpFlag = false;
	bool secondFlag = false;
	int jumpCount = 2;
	float g = 0.25;//重力
	float jumpSpeed = 0;

};

