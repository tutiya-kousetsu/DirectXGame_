#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"
#include <DirectXMath.h>
#include <memory>
class PlayerLife
{
public:
	void Initialize();
	void MoveUpdate(int life);
	void Draw(int life);

	inline bool GetDis() const { return disFlag; }
	inline void SetDis(bool disFlag) { this->disFlag = disFlag; }

private:

	DirectX::XMFLOAT2 pos[9]{};

	std::unique_ptr<Sprite> lifeSp[9]{};

	bool disFlag = false;

	int life = 0;
	int frame = 0.0f;
};