#pragma once
#include "GameObject.h"
#include "Fbx_Object3d.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "TouchableObject.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class Obstacle : TouchableObject
{
public:
	~Obstacle();

	void Initialize(DirectX::XMFLOAT3 pos);

	void Update();

	void Draw();

	void OnCollision();
public:
	inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

protected:
	DirectX::XMFLOAT3 position;
	std::unique_ptr<TouchableObject> object;
	//std::unique_ptr<Object3d> obj;
	bool alive = true;
	//std::unique_ptr<Material> material;
	DirectX:: XMFLOAT4 color;

};

