#pragma once
#include "Object3d.h"
class TouchableObject : public Object3d
{
public:
	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model = nullptr);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model* model);
	
	void Pop(DirectX::XMFLOAT3 position);

protected:
	std::unique_ptr<Object3d> object;
	DirectX::XMFLOAT3 position;
};

