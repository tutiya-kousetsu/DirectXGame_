#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "DirectXCommon.h"

class GameOver : public BaseScene
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(DirectXCommon* dxCommon) override;

	/// <summary>
	/// 終了
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(DirectXCommon* dxCommon) override;

private:

	//Sprite* sprite = nullptr;
	std::unique_ptr<Sprite> sprite;
};

