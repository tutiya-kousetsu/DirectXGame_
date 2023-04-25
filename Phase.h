#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>

class Phase
{
public:
	void Initialize();

	void MovePhase(int i);

	void Draw(int i);

	inline bool GetPhase() const { return phaseFlag; }
	inline void SetPhase(bool phaseFlag) { this->phaseFlag = phaseFlag; }
public:

private:
	DirectX::XMFLOAT3 pos[6]{};
	//Phase
	std::unique_ptr<Sprite> phase[6]{};

	//�C�[�Y�C���pframe
	float inFrame = 0.f;
	//�C�[�Y�A�E�g�pframe
	float outFrame = 0.f;
	bool phaseFlag = false;
};