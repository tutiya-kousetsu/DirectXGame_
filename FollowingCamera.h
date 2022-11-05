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

	// �g���b�L���O�^�[�Q�b�g����̃J�����̍���
	float CameraTargetHeight = 5;

	XMFLOAT3 CameraTarget = { 0,5,0 };

	// �J�����̎��_���璍���_�܂ł̋���
	float CameraTargetLength = 20;

};