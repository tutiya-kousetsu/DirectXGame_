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
		XMFLOAT3 target = followingTarget->GetPosition();

		float sinNum = sinf(XMConvertToRadians(followingTarget->GetRotation().x - 80));
		float cosNum = cosf(XMConvertToRadians(followingTarget->GetRotation().x - 80));

		// x����]�𔽉f�����ʒu
		XMFLOAT3 tempPosition = { 0,sinNum * CameraTargetLength ,cosNum * CameraTargetLength };

		sinNum = sinf(XMConvertToRadians(-followingTarget->GetRotation().y));
		cosNum = cosf(XMConvertToRadians(-followingTarget->GetRotation().y));

		// y����]�𔽉f�����ʒu
		XMFLOAT3 tempPosition2 = {
			cosNum * tempPosition.x - sinNum * tempPosition.y,
			tempPosition.z,
			sinNum * tempPosition.x + cosNum * tempPosition.y
		};

		XMFLOAT3 eye = {
		target.x + tempPosition2.x,
		target.y + tempPosition2.y,
		target.z + tempPosition2.z };

		// �ړ��O�̍��W
		XMFLOAT3 old = GetEye();
		// �ړ��� = �ړ���̍��W - �ړ��O�̍��W
		XMFLOAT3 velocity =
		{ (eye.x - old.x) * 1.5f,
		(eye.y - old.y) * 0.3f,
		(eye.z - old.z) * 1.5f };
		// �ړ���̍��W = �ړ��O�̍��W + �ړ���
		eye = { old.x + velocity.x,old.y + velocity.y ,old.z + velocity.z };
		// �ړ���̍��W��K�p
		SetEye(eye);

		XMFLOAT3 Destination;//�s���悫
		XMStoreFloat3(&Destination, XMVector3Transform(XMVectorSet(
			CameraTarget.x,
			CameraTarget.y,
			CameraTarget.z, 1),
			followingTarget->GetMatRotation()));

		target.x += Destination.x;
		target.y += Destination.y;
		target.z += Destination.z;
		SetTarget(target);
	}
}
