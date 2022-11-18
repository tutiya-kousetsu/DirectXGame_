#include "FollowingCamera.h"
#include "WinApp.h"

#include <DirectXMath.h>

using namespace DirectX;

FollowingCamera::FollowingCamera() :Camera(WinApp::window_width, WinApp::window_height)
{

}

void FollowingCamera::startUpdata()
{
	if (followingTarget) {
		XMVECTOR target = followingTarget->GetPosition();

		float sinNum = sinf(XMConvertToRadians(followingTarget->GetRotation().x - 75));
		float cosNum = cosf(XMConvertToRadians(followingTarget->GetRotation().x - 75));

		// x����]�𔽉f�����ʒu
		XMVECTOR reflectPosition = { 0,sinNum * CameraTargetLength ,cosNum * CameraTargetLength };

		sinNum = sinf(XMConvertToRadians(-followingTarget->GetRotation().y));
		cosNum = cosf(XMConvertToRadians(-followingTarget->GetRotation().y));

		// y����]�𔽉f�����ʒu
		XMVECTOR reflectPosition2 = {
			cosNum * reflectPosition.x - sinNum * reflectPosition.y,reflectPosition.z,
			sinNum * reflectPosition.x + cosNum * reflectPosition.y
		};

		//�^�[�Q�b�g�ɔ��f�����ʒu�𑫂�
		XMVECTOR eye = {
		target.x + reflectPosition2.x,
		target.y + reflectPosition2.y,
		target.z + reflectPosition2.z };

		// �ړ��O�̍��W
		XMVECTOR old = GetEye();
		// �ړ��� = �ړ���̍��W - �ړ��O�̍��W
		XMVECTOR velocity =
		{ (eye.x - old.x) * 1.5f,
		(eye.y - old.y) * 0.3f,
		(eye.z - old.z) * 1.5f };
		// �ړ���̍��W = �ړ��O�̍��W + �ړ���
		eye = { old.x + velocity.x,old.y + velocity.y ,old.z + velocity.z };
		// �ړ���̍��W��K�p
		SetEye(eye);

		XMVECTOR Destination;//�s���悫
		XMStoreFloat3(&Destination, XMVector3Transform(XMVectorSet(
			CameraTarget.x,CameraTarget.y,CameraTarget.z, 1),
			followingTarget->GetMatRotation()));

		target.x += Destination.x;
		target.y += Destination.y;
		target.z += Destination.z;
		SetTarget(target);
	}
}
