#include "FollowingCamera.h"
#include "WinApp.h"

#include <DirectXMath.h>

using namespace DirectX;

FollowingCamera::FollowingCamera() :Camera(WinApp::window_width, WinApp::window_height)
{

}

void FollowingCamera::startUpdate()
{
	if (followingTarget) {
		XMFLOAT3 target = followingTarget->GetPosition();

		float sinNum = sinf(XMConvertToRadians(followingTarget->GetRotation().x + 20));
		float cosNum = cosf(XMConvertToRadians(followingTarget->GetRotation().x + 20));

		// x軸回転を反映した位置
		XMFLOAT3 tempPosition = { 0,sinNum * CameraTargetLength ,-cosNum * CameraTargetLength };

		sinNum = sinf(XMConvertToRadians(-followingTarget->GetRotation().y));
		cosNum = cosf(XMConvertToRadians(-followingTarget->GetRotation().y));

		// y軸回転を反映した位置
		XMFLOAT3 tempPosition2 = {
			cosNum * tempPosition.x - sinNum * tempPosition.y,
			tempPosition.z,
			sinNum * tempPosition.x + cosNum * tempPosition.y
		};

		XMFLOAT3 eye = {
		target.x + tempPosition2.x,
		target.y + tempPosition2.y,
		target.z + tempPosition2.z };

		// 移動前の座標
		XMFLOAT3 old = GetEye();
		// 移動幅 = 移動後の座標 - 移動前の座標
		XMFLOAT3 velocity =
		{ (eye.x - old.x) * 0.3f,
		(eye.y - old.y) * 0.3f,
		(eye.z - old.z) * 0.3f };
		// 移動後の座標 = 移動前の座標 + 移動幅
		eye = { old.x + velocity.x,old.y + velocity.y ,old.z + velocity.z };
		// 移動後の座標を適用
		SetEye(eye);

		XMFLOAT3 Destination;//行き先き
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
