#pragma once

#include "Object3d.h"
#include "Sprite.h"
#include "DebugCamera.h"
#include <DirectXMath.h>

/// <summary>
/// ゲームプレイシーン
/// </summary>
class GamePlayScene
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	Sprite* sprite = nullptr;
	Model* modelPost = nullptr;
	Object3d* objPost = nullptr;
	DebugCamera* camera = nullptr;

	float speed = 0.75f;
	float t = 0.01f;
	float speed2 = 0.75f;
	double g = 0.0f;
	int Flag = 0;
	XMFLOAT3 playerPos;
};

