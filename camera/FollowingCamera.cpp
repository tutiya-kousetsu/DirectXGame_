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

		// x軸回転を反映した位置
		XMVECTOR reflectPosition = { 0,sinNum * CameraTargetLength ,cosNum * CameraTargetLength };

		sinNum = sinf(XMConvertToRadians(-followingTarget->GetRotation().y));
		cosNum = cosf(XMConvertToRadians(-followingTarget->GetRotation().y));

		// y軸回転を反映した位置
		XMVECTOR reflectPosition2 = {
			cosNum * reflectPosition.x - sinNum * reflectPosition.y,reflectPosition.z,
			sinNum * reflectPosition.x + cosNum * reflectPosition.y
		};

		//ターゲットに反映した位置を足す
		XMVECTOR eye = {
		target.x + reflectPosition2.x,
		target.y + reflectPosition2.y,
		target.z + reflectPosition2.z };

		// 移動前の座標
		XMVECTOR old = GetEye();
		// 移動幅 = 移動後の座標 - 移動前の座標
		XMVECTOR velocity =
		{ (eye.x - old.x) * 1.5f,
		(eye.y - old.y) * 0.3f,
		(eye.z - old.z) * 1.5f };
		// 移動後の座標 = 移動前の座標 + 移動幅
		eye = { old.x + velocity.x,old.y + velocity.y ,old.z + velocity.z };
		// 移動後の座標を適用
		SetEye(eye);

		XMVECTOR Destination;//行き先き
		XMStoreFloat3(&Destination, XMVector3Transform(XMVectorSet(
			CameraTarget.x,CameraTarget.y,CameraTarget.z, 1),
			followingTarget->GetMatRotation()));

		target.x += Destination.x;
		target.y += Destination.y;
		target.z += Destination.z;
		SetTarget(target);
	}
}
