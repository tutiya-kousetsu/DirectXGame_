#pragma once
#include <DirectXMath.h>
// 当たり判定プリミティブ

// 球
struct Sphere {
	// 中心座標
	DirectX::XMVECTOR center = { 0, 0, 0, 1 };
	// 半径
	float radius = 1.0f;
};

// 平面
struct Plane {
	// 法線ベクトル
	DirectX::XMVECTOR normal = { 0, 1, 0, 0 };
	// 原点(0, 0, 0)からの距離
	float distance = 0.0f;
};

// 法線付き三角形(時計回りが表)
class Triangle {
public:
	//頂点情報3つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;

	// 法線の計算
	void ComputeNormal();
};

// レイ(半直線)
struct Ray {
	// 始点
	DirectX::XMVECTOR start = { 0, 0, 0, 1 };
	DirectX::XMVECTOR end = { 1,1,1,1 };
	// 方向
	DirectX::XMVECTOR dir = { 1, 0, 0, 0 };
};