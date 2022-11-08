#pragma once
#include "Camera.h"
#include "GameObject.h"

class FollowingCamera : public Camera
{
public:
	using Camera::Camera;

	FollowingCamera();

	inline void SetFollowingTarget(GameObject* followingTarget) { this->followingTarget = followingTarget; }

	void startUpdate()override;

private:
	GameObject* followingTarget;

	// トラッキングターゲットからのカメラの高さ
	float CameraTargetHeight = 5;

	XMFLOAT3 CameraTarget = { 0,5,0 };

	// カメラの視点から注視点までの距離
	float CameraTargetLength = 20;

};