#pragma once
#include "GameObject.h"
#include "Fbx_Object3d.h"
#include "DirectXCommon.h"
#include "Object3d.h"
#include "TouchableObject.h"
#include "Shake.h"
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

class Obstacle : TouchableObject
{
public:
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public:
	~Obstacle();

	bool Initialize(XMFLOAT3 pos);

	void Update();

	void Draw();

	bool UpMove(bool landF = false);

	void OnCollision(bool flag = false);
public:
	inline void SetPosition(const XMFLOAT3& position) { object->SetPosition(position); }
	inline const XMFLOAT3& GetPosition() const { return object->GetPosition(); }

	inline void SetColor(const XMFLOAT4& position) { object->SetColor(color); }
	inline const XMFLOAT4& GetColor() const { return object->GetColor(); }

	inline void SetScale(const XMFLOAT3& scale) { object->SetScale(scale); }
	inline const XMFLOAT3& GetScale() const { return object->GetScale(); }

	inline void SetRotation(const XMFLOAT3& rotation) { object->SetRotation(rotation); }
	inline const XMFLOAT3& GetRotation() const { return object->GetRotation(); }

	inline bool GetAlive() const { return alive; }
	inline void SetAlive(bool alive) { this->alive = alive; }

protected:
	XMFLOAT3 position;
	std::unique_ptr<TouchableObject> object;
	bool alive = true;
	XMFLOAT4 color = {1.0f, 1.0f, 1.0f, 1.0f};
	float easFrame = 0.0f;
	//シェイク用
	std::unique_ptr<Shake> shake = nullptr;
	bool shakeF = false;
	XMFLOAT3 shakePos = { 0.0f,0.0f,0.0f };
};

