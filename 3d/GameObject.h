#pragma once

#include "Object3d.h"
#include "Fbx_Object3d.h"
#include "Model.h"
#include <DirectXMath.h>
#include <memory>

class GameObject
{
protected:
	std::unique_ptr<Object3d> object;
	bool alive = true;

public:

	inline bool GetAlive() const { return  alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

	inline void SetPosition(const DirectX::XMFLOAT3& position) { object->SetPosition(position); }
	inline const DirectX::XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMFLOAT3& scale) { object->SetScale(scale); }
	inline const DirectX::XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const DirectX::XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const DirectX::XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	inline const DirectX::XMMATRIX& GetMatRotation() const { return object->GetMatRot(); }
	inline void SetMatRotation(const DirectX::XMMATRIX& matRot) { object->SetMatRotation(matRot); }
	inline const DirectX::XMMATRIX& GetMatWorld() const { return object->GetMatWorld(); }

	GameObject(Model* model,const DirectX::XMFLOAT3& position = { 0,0,0 });

	virtual void Update();

	virtual void Draw();
};

