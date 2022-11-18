#pragma once

#include "Object3d.h"
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

	inline void SetPosition(const DirectX::XMVECTOR& position) { object->SetPosition(position); }
	inline const DirectX::XMVECTOR& GetPosition() const { return object->GetPosition(); }

	inline void SetScale(const DirectX::XMVECTOR& scale) { object->SetScale(scale); }
	inline const DirectX::XMVECTOR& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const DirectX::XMVECTOR& rotation) { object->SetRotation(rotation); }
	inline const DirectX::XMVECTOR& GetRotation() const { return object->GetRotation(); }

	inline const DirectX::XMMATRIX& GetMatRotation() const { return object->GetMatRot(); }

	GameObject(Model* model,const DirectX::XMVECTOR& position = { 0,0,0 });

	virtual void Updata();

	virtual void Draw();

};

