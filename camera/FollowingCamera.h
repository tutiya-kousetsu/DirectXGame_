#pragma once
#include "Camera.h"
#include "GameObject.h"

class FollowingCamera : public Camera
{
public:
	using Camera::Camera;

	//�R���X�g���N�^
	FollowingCamera();

	//�v���C���[�ɃZ�b�g����
	inline void SetFollowingTarget(GameObject* followingTarget) { this->followingTarget = followingTarget; }

	void startUpdata()override;

private:
	GameObject* followingTarget;

	// �^�[�Q�b�g����̃J�����̍���
	float CameraTargetHeight = 5;

	XMFLOAT3 CameraTarget = { 0,5,0 };

	// �J�����̎��_���璍���_�܂ł̋���
	float CameraTargetLength = 20;

};