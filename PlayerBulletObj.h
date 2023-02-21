#pragma once
#include "Object3d.h"
#include <DirectXMath.h>
#include <memory>

class PlayerBulletObj : public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static PlayerBulletObj* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);


protected:
	bool alive = true;
	std::unique_ptr<Object3d> object;
};

