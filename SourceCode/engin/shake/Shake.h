#pragma once
#include <Windows.h>
#include <wrl.h>
#include<d3d12.h>
#include <DirectXMath.h>

class Shake
{
public:
	void ShakePos(float& pos, const int shakeMax, const int shakeMin, const int TargetTim, const int Division);
public:
	bool GetShakeStart() { return shakeStart; }
	void SetShakeStart(bool shakeStart) { this->shakeStart = shakeStart; }
private:
	bool shakeStart = false;
	int32_t shakeTime = 0;
};

