#pragma once

#include "BaseCollider.h"
#include "CollisionPrimitive.h"

#include<DirectXMath.h>

class SphereCollider : public BaseCollider, public Sphere
{
private://エイリアス
	//DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
public:
	SphereCollider(XMVECTOR offset = {0,0,0,0},float radius = 1.0f):
};