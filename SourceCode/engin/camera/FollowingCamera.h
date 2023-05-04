#pragma once
#include "Camera.h"
#include "GameObject.h"

class FollowingCamera : public Camera
{
public:
	using Camera::Camera;

	//コンストラクタ
	FollowingCamera();

	//プレイヤーにセットする
	inline void SetFollowingTarget(Object3d* followingTarget) { this->followingTarget = followingTarget; }

	void startUpdate()override;

	//ワールド座標を取得
	XMVECTOR GetWorldPosition();

private:
	Object3d* followingTarget;

	// ターゲットからのカメラの高さ
	float CameraTargetHeight = 5;

	XMFLOAT3 CameraTarget = { 0,5,0 };

	// カメラの視点から注視点までの距離
	float CameraTargetLength = 20;

};