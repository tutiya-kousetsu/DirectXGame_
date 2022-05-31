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
	Object3d* objPost2 = nullptr;
	DebugCamera* camera = nullptr;

	float speed = 0.75f;
	float t = 0.01f;
	float speed2 = 0.15f;
	float g = 0.5f;
	int Flag = 0;
	float r;
	float m = 1.f;
	float repulCoe = 0.8f;    // 跳ね返り係数
	float Angle = 15.0f;             //角度
	float Length = 20.0f;                 //半径の長さ
	float centerX = 0;			//中心座標x
	float centerY = 0;			//中心座標y
	XMFLOAT3 playerPos;
	XMFLOAT3 playerScale;
	XMFLOAT3 playerPos2;
	XMFLOAT3 playerScale2;
	XMFLOAT3 v;
	XMFLOAT3 a;

};

